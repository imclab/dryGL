//
//  Shader.cpp
//  dryGL
//
//  Created by Jordi Ros on 15/02/13.
//  Copyright (c) 2013 Jordi Ros. All rights reserved.
//

#include "dry.h"
#include "Shader.h"

using namespace dry;


bool ReadFileContents(const string &filename, char **buffer)
{
    FILE *file = fopen(filename.c_str(), "r");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        fseek(file, 0, SEEK_SET);
        *buffer = NEW_ARRAY(char, size);
        fread(*buffer, 1, size, file);
        (*buffer)[size] = 0;
        fclose(file);
        return true;
    }
    return false;
}


//------------------------------------------------------------------------------------------------
// InitWithFile
//
//------------------------------------------------------------------------------------------------
bool Shader::InitWithFile(const string &vs, const string &fs)
{
    bool res = false;
    char *bvs = NULL;
    char *bfs = NULL;
    if (ReadFileContents(vs, &bvs) && ReadFileContents(fs, &bfs))
        res = InitWithProgram(bvs, bfs);
    DISPOSE_ARRAY(bvs);
    DISPOSE_ARRAY(bfs);
    return false;
}


//------------------------------------------------------------------------------------------------
// InitWithProgram
//
//------------------------------------------------------------------------------------------------
bool Shader::InitWithProgram(const string &vs, const string &fs)
{
    Free();
    bool res = true;
    
    const char *strVS = vs.c_str();
    const char *strFS = fs.c_str();
    int lenVS = strlen(strVS);
    int lenFS = strlen(strFS);
    int result;
    
    // Create VS
    m_HandleVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource (m_HandleVS, 1, &strVS, &lenVS);
    glCompileShader(m_HandleVS);
    glGetShaderiv  (m_HandleVS, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        LogShaderError(m_HandleVS, "vertex shader");
        res = false;
    }

    // Create FS
    m_HandleFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource (m_HandleFS, 1, &strFS, &lenFS);
    glCompileShader(m_HandleFS);
    glGetShaderiv  (m_HandleFS, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        LogShaderError(m_HandleFS, "fragment shader");
        res = false;
    }
    
    // Create Program
    if (res)
    {
        m_HandleProgram = glCreateProgram();
        glAttachShader(m_HandleProgram, m_HandleVS);
        glAttachShader(m_HandleProgram, m_HandleFS);
        glLinkProgram (m_HandleProgram);
        glGetProgramiv(m_HandleProgram, GL_LINK_STATUS, &result);
        if (result == GL_FALSE)
        {
            LogProgramError(m_HandleProgram, "program");
            res = false;
        }
    }
    
    // Clean?
    if (!res)
        Free();
    
    return res;
}


//------------------------------------------------------------------------------------------------
// Free
//
//------------------------------------------------------------------------------------------------
void Shader::Free()
{
    if (m_HandleProgram != -1) glDeleteProgram(m_HandleProgram);
    if (m_HandleVS != -1) glDeleteShader(m_HandleVS);
    if (m_HandleFS != -1) glDeleteShader(m_HandleFS);
    m_HandleProgram = -1;
    m_HandleVS = -1;
    m_HandleFS = -1;
}


//------------------------------------------------------------------------------------------------
// Bind
//
//------------------------------------------------------------------------------------------------
void Shader::Bind()
{
    glUseProgram(m_HandleProgram);
}


//------------------------------------------------------------------------------------------------
// Unbind
//
//------------------------------------------------------------------------------------------------
void Shader::Unbind()
{
    glUseProgram(0);
}


//------------------------------------------------------------------------------------------------
// LogShaderError
//
//------------------------------------------------------------------------------------------------
void Shader::LogShaderError(int handle, const string &info)
{
    char messages[256];
    glGetShaderInfoLog(handle, sizeof(messages), 0, &messages[0]);
    dry::Log(LOG_WARNING, "[dryShader] Error compiling %s", info.c_str());
    dry::Log(LOG_WARNING, messages);
}


//------------------------------------------------------------------------------------------------
// LogProgramError
//
//------------------------------------------------------------------------------------------------
void Shader::LogProgramError(int handle, const string &info)
{
    GLchar messages[256];
    glGetProgramInfoLog(handle, sizeof(messages), 0, &messages[0]);
    dry::Log(LOG_WARNING, "[dryShader] Error compiling %s", info.c_str());
    dry::Log(LOG_WARNING, messages);
}


//------------------------------------------------------------------------------------------------
// GetAttribLocation
//
//------------------------------------------------------------------------------------------------
int Shader::GetAttribLocation(const char *name)
{
    int attribute = glGetAttribLocation(m_HandleProgram, name);
    if (attribute < 0)
        dry::Log(LOG_WARNING, "[Shader] Can't find attribute %s", name);
    return attribute;
}


//------------------------------------------------------------------------------------------------
// GetUniformLocation
//
//------------------------------------------------------------------------------------------------
int Shader::GetUniformLocation(const char *name)
{
    int uniform = glGetUniformLocation(m_HandleProgram, name);
    if (uniform < 0)
        dry::Log(LOG_WARNING, "[Shader] Can't find uniform %s", name);
    return uniform;
}
