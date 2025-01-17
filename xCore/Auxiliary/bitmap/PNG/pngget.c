
/* pngget.c - retrieval of values from info struct
 *
 * libpng 1.0.5m - January 7, 2000
 * For conditions of distribution and use, see copyright notice in png.h
 * Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.
 * Copyright (c) 1996, 1997 Andreas Dilger
 * Copyright (c) 1998, 1999, 2000 Glenn Randers-Pehrson
 */

#define PNG_INTERNAL
#include "png.h"

xpng_uint_32
xpng_get_valid(xpng_structp xpng_ptr, xpng_infop info_ptr, xpng_uint_32 flag)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
      return(info_ptr->valid & flag);
   else
      return(0);
}

xpng_uint_32
xpng_get_rowbytes(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
      return(info_ptr->rowbytes);
   else
      return(0);
}

#ifdef PNG_EASY_ACCESS_SUPPORTED
/* easy access to info, added in libpng-0.99 */
xpng_uint_32
xpng_get_image_width(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
   {
      return info_ptr->width;
   }
   return (0);
}

xpng_uint_32
xpng_get_image_height(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
   {
      return info_ptr->height;
   }
   return (0);
}

xpng_byte
xpng_get_bit_depth(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
   {
      return info_ptr->bit_depth;
   }
   return (0);
}

xpng_byte
xpng_get_color_type(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
   {
      return info_ptr->color_type;
   }
   return (0);
}

xpng_byte
xpng_get_filter_type(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
   {
      return info_ptr->filter_type;
   }
   return (0);
}

xpng_byte
xpng_get_interlace_type(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
   {
      return info_ptr->interlace_type;
   }
   return (0);
}

xpng_byte
xpng_get_compression_type(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
   {
      return info_ptr->compression_type;
   }
   return (0);
}

xpng_uint_32
xpng_get_x_pixels_per_meter(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
#if defined(PNG_pHYs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_x_pixels_per_meter");
      if(info_ptr->phys_unit_type != PNG_RESOLUTION_METER)
          return (0);
      else return (info_ptr->x_pixels_per_unit);
   }
   else
#endif
   return (0);
}

xpng_uint_32
xpng_get_y_pixels_per_meter(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
#if defined(PNG_pHYs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_y_pixels_per_meter");
      if(info_ptr->phys_unit_type != PNG_RESOLUTION_METER)
          return (0);
      else return (info_ptr->y_pixels_per_unit);
   }
   else
#endif
   return (0);
}

xpng_uint_32
xpng_get_pixels_per_meter(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
#if defined(PNG_pHYs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_pixels_per_meter");
      if(info_ptr->phys_unit_type != PNG_RESOLUTION_METER ||
         info_ptr->x_pixels_per_unit != info_ptr->y_pixels_per_unit)
          return (0);
      else return (info_ptr->x_pixels_per_unit);
   }
   else
#endif
   return (0);
}

#ifdef PNG_FLOATING_POINT_SUPPORTED
float
xpng_get_pixel_aspect_ratio(xpng_structp xpng_ptr, xpng_infop info_ptr)
   {
#if defined(PNG_pHYs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_aspect_ratio");
      if (info_ptr->x_pixels_per_unit == 0)
         return ((float)0.0);
      else
         return ((float)info_ptr->y_pixels_per_unit
            /(float)info_ptr->x_pixels_per_unit);
   }
   else
#endif
      return ((float)0.0);
}
#endif

xpng_uint_32
xpng_get_x_offset_microns(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
#if defined(PNG_oFFs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_x_offset_microns");
      if(info_ptr->offset_unit_type != PNG_OFFSET_MICROMETER)
          return (0);
      else return (info_ptr->x_offset);
   }
   else
#endif
   return (0);
}

xpng_uint_32
xpng_get_y_offset_microns(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
#if defined(PNG_oFFs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_y_offset_microns");
      if(info_ptr->offset_unit_type != PNG_OFFSET_MICROMETER)
          return (0);
      else return (info_ptr->y_offset);
   }
   else
#endif
   return (0);
}

xpng_uint_32
xpng_get_x_offset_pixels(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
#if defined(PNG_oFFs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_x_offset_microns");
      if(info_ptr->offset_unit_type != PNG_OFFSET_PIXEL)
          return (0);
      else return (info_ptr->x_offset);
   }
   else
#endif
   return (0);
}

xpng_uint_32
xpng_get_y_offset_pixels(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
#if defined(PNG_oFFs_SUPPORTED)
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "xpng_get_y_offset_microns");
      if(info_ptr->offset_unit_type != PNG_OFFSET_PIXEL)
          return (0);
      else return (info_ptr->y_offset);
   }
   else
#endif
   return (0);
}

#if defined(PNG_INCH_CONVERSIONS) && defined(PNG_FLOATING_POINT_SUPPORTED)
xpng_uint_32
xpng_get_pixels_per_inch(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   return ((xpng_uint_32)((float)xpng_get_pixels_per_meter(xpng_ptr, info_ptr)
     *.03937 +.5)
}

xpng_uint_32
xpng_get_x_pixels_per_inch(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   return ((xpng_uint_32)((float)xpng_get_x_pixels_per_meter(xpng_ptr, info_ptr)
     *.03937 +.5)
}

xpng_uint_32
xpng_get_y_pixels_per_inch(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   return ((xpng_uint_32)((float)xpng_get_y_pixels_per_meter(xpng_ptr, info_ptr)
     *.03937 +.5)
}

float
xpng_get_x_offset_inches(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   return ((float)xpng_get_x_offset_microns(xpng_ptr, info_ptr)
     *.03937/1000000. +.5)
}

float
xpng_get_y_offset_inches(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   return ((float)xpng_get_y_offset_microns(xpng_ptr, info_ptr)
     *.03937/1000000. +.5)
}

#if defined(PNG_READ_pHYs_SUPPORTED)
xpng_uint_32
xpng_get_pHYs_dpi(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_uint_32 *res_x, xpng_uint_32 *res_y, int *unit_type)
{
   xpng_uint_32 retval = 0;

   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "pHYs");
      if (res_x != NULL)
      {
         *res_x = info_ptr->x_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }
      if (res_y != NULL)
      {
         *res_y = info_ptr->y_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }
      if (unit_type != NULL)
      {
         *unit_type = (int)info_ptr->phys_unit_type;
         retval |= PNG_INFO_pHYs;
         if(unit_type == 1)
         {
            if (res_x != NULL) *res_x = (xpng_uint_32)(*res_x * 39.37 + .50);
            if (res_y != NULL) *res_y = (xpng_uint_32)(*res_y * 39.37 + .50);
         }
      }
   }
   return (retval);
}
#endif /* PNG_READ_pHYs_SUPPORTED */
#endif  /* PNG_INCH_CONVERSIONS && PNG_FLOATING_POINT_SUPPORTED */

/* xpng_get_channels really belongs in here, too, but it's been around longer */

#endif  /* PNG_EASY_ACCESS_SUPPORTED */

xpng_byte
xpng_get_channels(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
      return(info_ptr->channels);
   else
      return (0);
}

xpng_bytep
xpng_get_signature(xpng_structp xpng_ptr, xpng_infop info_ptr)
{
   if (xpng_ptr != NULL && info_ptr != NULL)
      return(info_ptr->signature);
   else
      return (NULL);
}

#if defined(PNG_READ_bKGD_SUPPORTED)
xpng_uint_32
xpng_get_bKGD(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_color_16p *background)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_bKGD)
      && background != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "bKGD");
      *background = &(info_ptr->background);
      return (PNG_INFO_bKGD);
   }
   return (0);
}
#endif

#if defined(PNG_READ_cHRM_SUPPORTED)
#ifdef PNG_FLOATING_POINT_SUPPORTED
xpng_uint_32
xpng_get_cHRM(xpng_structp xpng_ptr, xpng_infop info_ptr,
   double *white_x, double *white_y, double *red_x, double *red_y,
   double *green_x, double *green_y, double *blue_x, double *blue_y)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
   {
      xpng_debug1(1, "in %s retrieval function\n", "cHRM");
      if (white_x != NULL)
         *white_x = (double)info_ptr->x_white;
      if (white_y != NULL)
         *white_y = (double)info_ptr->y_white;
      if (red_x != NULL)
         *red_x = (double)info_ptr->x_red;
      if (red_y != NULL)
         *red_y = (double)info_ptr->y_red;
      if (green_x != NULL)
         *green_x = (double)info_ptr->x_green;
      if (green_y != NULL)
         *green_y = (double)info_ptr->y_green;
      if (blue_x != NULL)
         *blue_x = (double)info_ptr->x_blue;
      if (blue_y != NULL)
         *blue_y = (double)info_ptr->y_blue;
      return (PNG_INFO_cHRM);
   }
   return (0);
}
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
xpng_uint_32
xpng_get_cHRM_fixed(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_fixed_point *white_x, xpng_fixed_point *white_y, xpng_fixed_point *red_x,
   xpng_fixed_point *red_y, xpng_fixed_point *green_x, xpng_fixed_point *green_y,
   xpng_fixed_point *blue_x, xpng_fixed_point *blue_y)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
   {
      xpng_debug1(1, "in %s retrieval function\n", "cHRM");
      if (white_x != NULL)
         *white_x = info_ptr->int_x_white;
      if (white_y != NULL)
         *white_y = info_ptr->int_y_white;
      if (red_x != NULL)
         *red_x = info_ptr->int_x_red;
      if (red_y != NULL)
         *red_y = info_ptr->int_y_red;
      if (green_x != NULL)
         *green_x = info_ptr->int_x_green;
      if (green_y != NULL)
         *green_y = info_ptr->int_y_green;
      if (blue_x != NULL)
         *blue_x = info_ptr->int_x_blue;
      if (blue_y != NULL)
         *blue_y = info_ptr->int_y_blue;
      return (PNG_INFO_cHRM);
   }
   return (0);
}
#endif
#endif

#if defined(PNG_READ_gAMA_SUPPORTED)
#ifdef PNG_FLOATING_POINT_SUPPORTED
xpng_uint_32
xpng_get_gAMA(xpng_structp xpng_ptr, xpng_infop info_ptr, double *file_gamma)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA)
      && file_gamma != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "gAMA");
      *file_gamma = (double)info_ptr->gamma;
      return (PNG_INFO_gAMA);
   }
   return (0);
}
#endif
xpng_uint_32
xpng_get_gAMA_fixed(xpng_structp xpng_ptr, xpng_infop info_ptr,
    xpng_fixed_point *int_file_gamma)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA)
      && int_file_gamma != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "gAMA");
      *int_file_gamma = info_ptr->int_gamma;
      return (PNG_INFO_gAMA);
   }
   return (0);
}
#endif

#if defined(PNG_READ_sRGB_SUPPORTED)
xpng_uint_32
xpng_get_sRGB(xpng_structp xpng_ptr, xpng_infop info_ptr, int *file_srgb_intent)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB)
      && file_srgb_intent != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "sRGB");
      *file_srgb_intent = (int)info_ptr->srgb_intent;
      return (PNG_INFO_sRGB);
   }
   return (0);
}
#endif

#if defined(PNG_READ_iCCP_SUPPORTED)
xpng_uint_32
xpng_get_iCCP(xpng_structp xpng_ptr, xpng_infop info_ptr,
             xpng_charpp name, int *compression_type,
             xpng_charpp profile, xpng_uint_32 *proflen)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_iCCP)
      && name != NULL && profile != NULL && proflen != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "iCCP");
      *name = info_ptr->iccp_name;
      *profile = info_ptr->iccp_profile;
      /* compression_type is a dummy so the API won't have to change
         if we introduce multiple compression types later. */
      *proflen = (int)info_ptr->iccp_proflen;
      *compression_type = (int)info_ptr->iccp_compression;
      return (PNG_INFO_iCCP);
   }
   return (0);
}
#endif

#if defined(PNG_READ_sPLT_SUPPORTED)
xpng_uint_32
xpng_get_spalettes(xpng_structp xpng_ptr, xpng_infop info_ptr,
             xpng_spalette_pp spalettes)
{
   if (xpng_ptr != NULL && info_ptr != NULL && spalettes != NULL)
     *spalettes = info_ptr->splt_palettes;
   return ((xpng_uint_32)info_ptr->splt_palettes_num);
}
#endif

#if defined(PNG_READ_hIST_SUPPORTED)
xpng_uint_32
xpng_get_hIST(xpng_structp xpng_ptr, xpng_infop info_ptr, xpng_uint_16p *hist)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_hIST)
      && hist != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "hIST");
      *hist = info_ptr->hist;
      return (PNG_INFO_hIST);
   }
   return (0);
}
#endif

xpng_uint_32
xpng_get_IHDR(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_uint_32 *width, xpng_uint_32 *height, int *bit_depth,
   int *color_type, int *interlace_type, int *compression_type,
   int *filter_type)

{
   if (xpng_ptr != NULL && info_ptr != NULL && width != NULL && height != NULL &&
      bit_depth != NULL && color_type != NULL)
   {
      int pixel_depth, channels;
      xpng_uint_32 rowbytes_per_pixel;

      xpng_debug1(1, "in %s retrieval function\n", "IHDR");
      *width = info_ptr->width;
      *height = info_ptr->height;
      *bit_depth = info_ptr->bit_depth;
      *color_type = info_ptr->color_type;
      if (compression_type != NULL)
         *compression_type = info_ptr->compression_type;
      if (filter_type != NULL)
         *filter_type = info_ptr->filter_type;
      if (interlace_type != NULL)
         *interlace_type = info_ptr->interlace_type;

      /* check for potential overflow of rowbytes */
      if (*color_type == PNG_COLOR_TYPE_PALETTE)
         channels = 1;
      else if (*color_type & PNG_COLOR_MASK_COLOR)
         channels = 3;
      else
         channels = 1;
      if (*color_type & PNG_COLOR_MASK_ALPHA)
         channels++;
      pixel_depth = *bit_depth * channels;
      rowbytes_per_pixel = (pixel_depth + 7) >> 3;
      if ((*width > PNG_MAX_UINT/rowbytes_per_pixel))
      {
         xpng_warning(xpng_ptr,
            "Width too large for libpng to process image data.");
      }
      return (1);
   }
   return (0);
}

#if defined(PNG_READ_oFFs_SUPPORTED)
xpng_uint_32
xpng_get_oFFs(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_int_32 *offset_x, xpng_int_32 *offset_y, int *unit_type)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs)
      && offset_x != NULL && offset_y != NULL && unit_type != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "oFFs");
      *offset_x = info_ptr->x_offset;
      *offset_y = info_ptr->y_offset;
      *unit_type = (int)info_ptr->offset_unit_type;
      return (PNG_INFO_oFFs);
   }
   return (0);
}
#endif

#if defined(PNG_READ_pCAL_SUPPORTED)
xpng_uint_32
xpng_get_pCAL(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_charp *purpose, xpng_int_32 *X0, xpng_int_32 *X1, int *type, int *nparams,
   xpng_charp *units, xpng_charpp *params)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pCAL)
      && purpose != NULL && X0 != NULL && X1 != NULL && type != NULL &&
      nparams != NULL && units != NULL && params != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "pCAL");
      *purpose = info_ptr->pcal_purpose;
      *X0 = info_ptr->pcal_X0;
      *X1 = info_ptr->pcal_X1;
      *type = (int)info_ptr->pcal_type;
      *nparams = (int)info_ptr->pcal_nparams;
      *units = info_ptr->pcal_units;
      *params = info_ptr->pcal_params;
      return (PNG_INFO_pCAL);
   }
   return (0);
}
#endif

#if defined(PNG_READ_sCAL_SUPPORTED) || defined(PNG_WRITE_sCAL_SUPPORTED)
#ifdef PNG_FLOATING_POINT_SUPPORTED
xpng_uint_32
xpng_get_sCAL(xpng_structp xpng_ptr, xpng_infop info_ptr,
             int *unit, double *width, double *height)
{
    if (xpng_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_sCAL))
    {
        *unit = info_ptr->scal_unit;
        *width = info_ptr->scal_pixel_width;
        *height = info_ptr->scal_pixel_height;
        return (PNG_INFO_sCAL);
    }
    return(0);
}
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
xpng_uint_32
xpng_get_sCAL_s(xpng_structp xpng_ptr, xpng_infop info_ptr,
             int *unit, xpng_charpp width, xpng_charpp height)
{
    if (xpng_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_sCAL))
    {
        *unit = info_ptr->scal_unit;
        *width = info_ptr->scal_s_width;
        *height = info_ptr->scal_s_height;
        return (PNG_INFO_sCAL);
    }
    return(0);
}
#endif
#endif
#endif

#if defined(PNG_READ_pHYs_SUPPORTED)
xpng_uint_32
xpng_get_pHYs(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_uint_32 *res_x, xpng_uint_32 *res_y, int *unit_type)
{
   xpng_uint_32 retval = 0;

   if (xpng_ptr != NULL && info_ptr != NULL &&
      (info_ptr->valid & PNG_INFO_pHYs))
   {
      xpng_debug1(1, "in %s retrieval function\n", "pHYs");
      if (res_x != NULL)
      {
         *res_x = info_ptr->x_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }
      if (res_y != NULL)
      {
         *res_y = info_ptr->y_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }
      if (unit_type != NULL)
      {
         *unit_type = (int)info_ptr->phys_unit_type;
         retval |= PNG_INFO_pHYs;
      }
   }
   return (retval);
}
#endif

xpng_uint_32
xpng_get_PLTE(xpng_structp xpng_ptr, xpng_infop info_ptr, xpng_colorp *palette,
   int *num_palette)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_PLTE)
       && palette != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "PLTE");
      *palette = info_ptr->palette;
      *num_palette = info_ptr->num_palette;
      xpng_debug1(3, "num_palette = %d\n", *num_palette);
      return (PNG_INFO_PLTE);
   }
   return (0);
}

#if defined(PNG_READ_sBIT_SUPPORTED)
xpng_uint_32
xpng_get_sBIT(xpng_structp xpng_ptr, xpng_infop info_ptr, xpng_color_8p *sig_bit)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_sBIT)
      && sig_bit != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "sBIT");
      *sig_bit = &(info_ptr->sig_bit);
      return (PNG_INFO_sBIT);
   }
   return (0);
}
#endif

#if defined(PNG_READ_TEXT_SUPPORTED)
xpng_uint_32
xpng_get_text(xpng_structp xpng_ptr, xpng_infop info_ptr, xpng_textp *text_ptr,
   int *num_text)
{
   if (xpng_ptr != NULL && info_ptr != NULL && info_ptr->num_text > 0)
   {
      xpng_debug1(1, "in %s retrieval function\n",
         (xpng_ptr->chunk_name[0] == '\0' ? "text"
             : (xpng_const_charp)xpng_ptr->chunk_name));
      if (text_ptr != NULL)
         *text_ptr = info_ptr->text;
      if (num_text != NULL)
         *num_text = info_ptr->num_text;
      return ((xpng_uint_32)info_ptr->num_text);
   }
   return(0);
}
#endif

#if defined(PNG_READ_tIME_SUPPORTED)
xpng_uint_32
xpng_get_tIME(xpng_structp xpng_ptr, xpng_infop info_ptr, xpng_timep *mod_time)
{
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_tIME)
       && mod_time != NULL)
   {
      xpng_debug1(1, "in %s retrieval function\n", "tIME");
      *mod_time = &(info_ptr->mod_time);
      return (PNG_INFO_tIME);
   }
   return (0);
}
#endif

#if defined(PNG_READ_tRNS_SUPPORTED)
xpng_uint_32
xpng_get_tRNS(xpng_structp xpng_ptr, xpng_infop info_ptr,
   xpng_bytep *trans, int *num_trans, xpng_color_16p *trans_values)
{
   xpng_uint_32 retval = 0;
   if (xpng_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS))
   {
      xpng_debug1(1, "in %s retrieval function\n", "tRNS");
      if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
          if (trans != NULL)
          {
             *trans = info_ptr->trans;
             retval |= PNG_INFO_tRNS;
          }
          if (trans_values != NULL)
             *trans_values = &(info_ptr->trans_values);
      }
      else /* if (info_ptr->color_type != PNG_COLOR_TYPE_PALETTE) */
      {
          if (trans_values != NULL)
          {
             *trans_values = &(info_ptr->trans_values);
             retval |= PNG_INFO_tRNS;
          }
          if(trans != NULL)
             *trans = NULL;
      }
      if(num_trans != NULL)
      {
         *num_trans = info_ptr->num_trans;
         retval |= PNG_INFO_tRNS;
      }
   }
   return (retval);
}
#endif

#if defined(PNG_READ_UNKNOWN_CHUNKS_SUPPORTED)
xpng_uint_32
xpng_get_unknown_chunks(xpng_structp xpng_ptr, xpng_infop info_ptr,
             xpng_unknown_chunkpp unknowns)
{
   if (xpng_ptr != NULL && info_ptr != NULL && unknowns != NULL)
     *unknowns = info_ptr->unknown_chunks;
   return ((xpng_uint_32)info_ptr->unknown_chunks_num);
}
#endif

#if defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
xpng_byte
xpng_get_rgb_to_gray_status (xpng_structp xpng_ptr)
{
   return xpng_ptr->rgb_to_gray_status;
}
#endif

#if defined(PNG_READ_USER_CHUNKS_SUPPORTED)
xpng_voidp
xpng_get_user_chunk_ptr(xpng_structp xpng_ptr)
{
   return (xpng_ptr->user_chunk_ptr);
}
#endif

