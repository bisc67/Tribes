//==============================================================================
//
//  LabelParse.hpp
//
//==============================================================================
#ifndef __LABELPARSE_HPP
#define __LABELPARSE_HPP

//=============================================================================
//  Includes
//=============================================================================
#include "x_files.hpp"

//=============================================================================
//  label class
//=============================================================================
class label_data
{
    
public:

    xstring     m_Label;
    s32         m_ID;
    xstring     m_Desc;

};

//=============================================================================
//  labels class
//=============================================================================

class labels
{

public:
    labels()    {};
    ~labels()   {};

    xbool   LoadLabelSet( const char *pFileName );
    s32     FindLabel( const char *pLabel );

private:

    xstring             m_Name;
    xarray<label_data>  m_Labels;    
};




#endif