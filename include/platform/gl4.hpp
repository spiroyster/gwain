#ifndef GWAIN_GL4_HPP
#define GWAIN_GL4_HPP

#include "..\core\event.hpp"
#include "..\core\texture.hpp"
#include "..\core\instance.hpp"

#include <gl/GL.h>

// GL Function macros...
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "glext.h"
#include "wglext.h"

#define GLFN_PROTOTYPE(prototype) PFN ## prototype ## PROC
#define GLFN_DECLARE(prototype, name) GLFN_PROTOTYPE(prototype) name;
#define GLFN_DEFINE(prototype, name) name = (GLFN_PROTOTYPE(prototype))wglGetProcAddress(#name);
#define GLFN(prototype, name) GLFN_PROTOTYPE(prototype) name = (GLFN_PROTOTYPE(prototype))wglGetProcAddress(#name);
#endif

namespace gwain
{
	class gl4 : 
		public core::instance_interface::display_adaptor,
		public eos_observer::texture_registered,
		public eos_observer::texture_unregistered,
		public eos_observer::texture_image
	{
		// textures (first = dirty, second = gl id)...
		std::map<texture_id, std::pair<bool, GLuint>> textures_;

		// quad geometry...
		GLuint program_ = 0;
		GLuint program_P_ = 0; 
		GLuint program_display_ = 0;
		GLuint program_position_ = 0;
		GLuint program_wh_ = 0;
		GLuint program_texture_ = 0;

		// list of textures to remove...
		std::list<texture_id> textures_requiring_removal_;
		
		GLFN_DECLARE(GLBINDVERTEXARRAY, glBindVertexArray)
		GLFN_DECLARE(GLBINDBUFFER, glBindBuffer)
		GLFN_DECLARE(GLBUFFERDATA, glBufferData)
		GLFN_DECLARE(GLACTIVETEXTURE, glActiveTexture)
		GLFN_DECLARE(GLGETUNIFORMLOCATION, glGetUniformLocation)
		GLFN_DECLARE(GLUNIFORM1I, glUniform1i)
		GLFN_DECLARE(GLUNIFORM2I, glUniform2i)
		GLFN_DECLARE(GLUNIFORM3F, glUniform3f)
		GLFN_DECLARE(GLUNIFORM4F, glUniform4f)
		GLFN_DECLARE(GLUNIFORMMATRIX4FV, glUniformMatrix4fv)
		GLFN_DECLARE(GLUSEPROGRAM, glUseProgram)

		GLuint quad_ = 0, points_, uvs_, indicies_;

		GLuint glsl_compile(GLuint type, const std::string& source)
		{
			GLFN(GLCREATESHADER, glCreateShader)
			GLFN(GLSHADERSOURCE, glShaderSource)
			GLFN(GLCOMPILESHADER, glCompileShader)
			GLFN(GLGETSHADERIV, glGetShaderiv)
			GLFN(GLGETSHADERINFOLOG, glGetShaderInfoLog)

			GLuint shaderID = glCreateShader(type);
			const char* src = source.c_str();

			glShaderSource(shaderID, 1, &src, NULL);
			glCompileShader(shaderID);

			GLint result = GL_FALSE;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				int infoLogLength;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
				std::vector<GLchar> error(infoLogLength);
				glGetShaderInfoLog(shaderID, infoLogLength, &infoLogLength, &error[0]);
				throw std::runtime_error(std::string(&error[0], error.size()));
			}
			return shaderID;
		}

		GLuint glsl_link(const std::vector<GLuint>& shaders)
		{
			GLFN(GLCREATEPROGRAM, glCreateProgram)
			GLFN(GLATTACHSHADER, glAttachShader)
			GLFN(GLDETACHSHADER, glDetachShader)
			GLFN(GLLINKPROGRAM, glLinkProgram)
			GLFN(GLGETPROGRAMIV, glGetProgramiv)
			GLFN(GLGETPROGRAMINFOLOG, glGetProgramInfoLog)

			GLuint programID = glCreateProgram();
			for (unsigned int s = 0; s < shaders.size(); ++s)
				glAttachShader(programID, shaders[s]);

			glLinkProgram(programID);
			GLint result = GL_FALSE;
			glGetProgramiv(programID, GL_LINK_STATUS, &result);
			if (result == GL_FALSE)
			{
				int InfoLogLength;
				glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
				std::vector<GLchar> error(InfoLogLength);
				glGetProgramInfoLog(programID, InfoLogLength, &InfoLogLength, &error[0]);
				throw std::runtime_error(std::string(&error[0], error.size()));
			}

			for (unsigned int s = 0; s < shaders.size(); ++s)
				glDetachShader(programID, shaders[s]);

			return programID;
		}

		void initialise()
		{
			GLFN_DEFINE(GLBINDVERTEXARRAY, glBindVertexArray)
			GLFN_DEFINE(GLBINDBUFFER, glBindBuffer)
			GLFN_DEFINE(GLBUFFERDATA, glBufferData)
			GLFN_DEFINE(GLACTIVETEXTURE, glActiveTexture)
			GLFN_DEFINE(GLGETUNIFORMLOCATION, glGetUniformLocation)
			GLFN_DEFINE(GLUNIFORM1I, glUniform1i)
			GLFN_DEFINE(GLUNIFORM2I, glUniform2i)
			GLFN_DEFINE(GLUNIFORM3F, glUniform3f)
			GLFN_DEFINE(GLUNIFORM4F, glUniform4f)
			GLFN_DEFINE(GLUNIFORMMATRIX4FV, glUniformMatrix4fv)
			GLFN_DEFINE(GLUSEPROGRAM, glUseProgram)

			GLFN(GLGENVERTEXARRAYS, glGenVertexArrays)
			GLFN(GLGENBUFFERS, glGenBuffers)
			GLFN(GLENABLEVERTEXATTRIBARRAY, glEnableVertexAttribArray)
			GLFN(GLVERTEXATTRIBPOINTER, glVertexAttribPointer)
			GLFN(GLDELETESHADER, glDeleteShader)

			std::vector<GLfloat> uvs = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
			std::vector<GLfloat> points = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
			//std::vector<GLfloat> points = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0, 1.0f, 0.0f };
			std::vector<GLuint> indicies = { 0, 1, 2, 2, 3, 0 };

			glGenVertexArrays(1, &quad_);
			glBindVertexArray(quad_);

			// points...
			glGenBuffers(1, &points_);
			glBindBuffer(GL_ARRAY_BUFFER, points_);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), &points.front(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// uvs...
			glGenBuffers(1, &uvs_);
			glBindBuffer(GL_ARRAY_BUFFER, uvs_);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvs.size(), &uvs.front(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			// indicies...
			glGenBuffers(1, &indicies_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies_);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies.size(), &indicies.front(), GL_STATIC_DRAW);
			
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
			glBindVertexArray(NULL);

			// Compile the shaders and create the program...
			std::string vertex_shader_source = R"(
				#version 430 core
				layout(location = 0) in vec3 InPoint;
				layout(location = 1) in vec2 InUV;
				uniform vec4 display;
				uniform ivec2 wh;
				uniform vec3 position;
				out vec2 UV;
				void main()
				{
					float vp_width = 2.0 / display.x;
					float vp_height = 2.0 / display.y;
					float vp_depth = 2.0 / (display.w - display.z);

					vec3 point = (InPoint + vec3(1.0)) * 0.5;

					// scale...
					point *= vec3(float(wh.x) * vp_width, float(wh.y) * vp_height, 1.0);

					// translate...
					point += vec3(-1.0 + (position.x * vp_width), -1.0 + (position.y * vp_height), position.z * vp_depth);
					
					gl_Position = vec4(point, 1.0);
					UV = InUV;
				}
				)";
			GLuint vertex_shader = glsl_compile(GL_VERTEX_SHADER, vertex_shader_source);

			std::string fragment_shader_source = R"(
				#version 430 core
				uniform sampler2D image;
				in vec2 UV;
				out vec4 frag;
				void main()
				{
					frag = texture(image, UV);
				}
				)";
			GLuint fragment_shader = glsl_compile(GL_FRAGMENT_SHADER, fragment_shader_source);

			program_ = glsl_link({ vertex_shader, fragment_shader });

			// delete shaders...
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			// get the uniform location...
			program_display_ = glGetUniformLocation(program_, "display"); 
			program_position_ = glGetUniformLocation(program_, "position");
			program_wh_ = glGetUniformLocation(program_, "wh");
			program_texture_ = glGetUniformLocation(program_, "image");
		}

		void recache_texture(GLuint& tex, const gwain::image& img)
		{
			if (!tex)
				glGenTextures(1, &tex);

			glBindTexture(GL_TEXTURE_2D, tex);
			switch (img.pixel_stride())
			{
			case 3:     // 24bit RGB (byte)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.image_width(), img.image_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, static_cast<const unsigned char*>(img.image_raw()));
				break;
			case 4:     // 32bit RGBA (byte)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.image_width(), img.image_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const unsigned char*>(img.image_raw()));
				break;
			case 12:     // 96bit RGB (float)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.image_width(), img.image_height(), 0, GL_RGB, GL_FLOAT, static_cast<const float*>(img.image_raw()));
				break;
			case 16:     // 128bit RGBA
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.image_width(), img.image_height(), 0, GL_RGBA, GL_FLOAT, static_cast<const float*>(img.image_raw()));
				break;
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, NULL);
		}

		class raii
		{
			GLenum cap_;
			GLboolean restore_;
		public:
			raii(GLenum cap)
				: cap_(cap)
			{
				glGetBooleanv(cap_, &restore_);
				glEnable(cap_);
			}

			~raii()
			{
				if (restore_)
					glEnable(cap_);
				else
					glDisable(cap_);
			}
		};

	public:
		
		void render_frame(const dim& display, const std::vector<view*>& views)
		{
			// check if has been initialised...
			if (!program_)
				initialise();

			// remove any textures...
			while (!textures_requiring_removal_.empty())
			{
				auto tex_itr = textures_.find(textures_requiring_removal_.front());
				if (tex_itr != textures_.end())
				{
					glDeleteTextures(1, &tex_itr->second.second);
					textures_.erase(tex_itr);
				}
				textures_requiring_removal_.pop_front();
			}

			// clear depth buffer for the display...
			glViewport(0, 0, display.wh_.x_, display.wh_.y_);
			glClear(GL_DEPTH_BUFFER_BIT);

			// enable modes...
			raii scissor(GL_SCISSOR_TEST);
			raii blend(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glUseProgram(program_);
			glUniform4f(program_display_, static_cast<float>(display.wh_.x_), static_cast<float>(display.wh_.y_), 0.001f, 100.0f);
			
			// for each active view...
			for (auto view_itr = views.begin(); view_itr != views.end(); ++view_itr)
			{
				//if ((*view_itr)->view_region() == mask_auto)
				//	glViewport((*view_itr)->view_region().xy_.x_, (*view_itr)->view_region().xy_.y_, (*view_itr)->view_region().wh_.x_, (*view_itr)->view_region().wh_.y_);

				// get the controls to render...
				for (auto scene_itr = (*view_itr)->view_scenes().begin(); scene_itr != (*view_itr)->view_scenes().end(); ++scene_itr)
				{
					const std::list<const control*>& scene_controls = gwain::instance().scene_controls(*scene_itr);
					if (scene_controls.empty())
						continue;

					for (const auto* control_itr : scene_controls)
					{
						// apply the scissor...
						const dim& d = control_itr->control_mask() == mask_auto ? control_itr->control_region() : control_itr->control_mask();
						glScissor(d.xy_.x_, d.xy_.y_, d.wh_.x_, d.wh_.x_);

						// check the texture...
						//auto tex_itr = textures_.find(control_itr->control_appearance().texture_->texture_name());
						auto tex_itr = textures_.find(control_itr->control_texture().texture_name());
						if (tex_itr->second.first)
						{
							recache_texture(tex_itr->second.second, control_itr->control_texture().texture_image());
							tex_itr->second.first = false;
						}

						// set the texture...
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, tex_itr->second.second);

						glUniform2i(program_wh_, control_itr->control_region().wh_.x_, control_itr->control_region().wh_.y_);
						glUniform3f(program_position_, static_cast<float>(control_itr->control_region().xy_.x_), static_cast<float>(control_itr->control_region().xy_.y_), control_itr->control_depth());
						glUniform1i(program_texture_, 0);

						// draw...
						glBindVertexArray(quad_);
						glBindBuffer(GL_ARRAY_BUFFER, points_);
						glBindBuffer(GL_ARRAY_BUFFER, uvs_);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies_);

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

						glBindVertexArray(NULL);
						glBindBuffer(GL_ARRAY_BUFFER, NULL);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
						glBindTexture(GL_TEXTURE_2D, NULL);
					}
				}
			}
		}

		void display_adaptor_free() 
		{
			// free any textures left...
			for (const auto& tex : textures_)
				glDeleteTextures(1, &tex.second.second);

			// free the quad...


			// free the program...
			//glDeleteProgram(program_);
		}

		void texture_registered_event(const texture* t)
		{
			auto itr = textures_.find(t->texture_name());
			if (itr != textures_.end())
				throw exception(error::textureAlreadyRegistered, t->texture_name());
			textures_[t->texture_name()] = std::make_pair(true, 0);
		}

		void texture_unregistered_event(const texture* t)
		{
			textures_requiring_removal_.emplace_back(t->texture_name());
		}
			
		void texture_image_event(const texture* t)
		{
			auto itr = textures_.find(t->texture_name());
			if (itr == textures_.end())
				throw exception(error::textureNotFound, t->texture_name());
			itr->second.first = true;
		}

	};
}

#endif // GWAIN_GL4_HPP