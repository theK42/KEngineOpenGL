//
//  TextureFactory_iOS.m
//  KEngineOpenGL
//
//  Created by Kelson Hootman on 1/15/18.
//  Copyright © 2018 Kelson Hootman. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#include "TextureFactory.h"

void KEngineOpenGL::TextureFactory::Init()
{
    mTextures.clear();
}

void KEngineOpenGL::TextureFactory::CreateTexture(KEngineCore::StringHash name, const std::string & textureFilename)
{
    NSString * nsFilename = [NSString stringWithCString:textureFilename.c_str() encoding:[NSString defaultCStringEncoding]];
    
    GLKTextureInfo *spriteTexture;
    NSError *theError;
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:nsFilename ofType:nil];
    
    spriteTexture = [GLKTextureLoader textureWithContentsOfFile:filePath options:nil error:&theError];
    
    mTextures[name] = spriteTexture.name;
}

const GLuint KEngineOpenGL::TextureFactory::GetTexture(KEngineCore::StringHash name)
{
    return mTextures[name];
}
