/*
 * exempi - xmp.h
 *
 * Copyright (C) 2007-2023 Hubert Figuière
 * Copyright 2002-2007 Adobe Systems Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1 Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2 Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * 3 Neither the name of the Authors, nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software wit hout specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __EXEMPI_XMP_H_
#define __EXEMPI_XMP_H_

#include <stdlib.h>
/* stdbool choke on Sun (bug #14612) */
#if !(defined(__sun) && defined(__cplusplus))
#include <stdbool.h>
#endif
#include <stdint.h>

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* enums grafted from XMP_Const.h */

/** Option bits for xmp_files_open() */
typedef enum {
    XMP_OPEN_NOOPTION = 0x00000000,        /**< No open option */
    XMP_OPEN_READ = 0x00000001,            /**< Open for read-only access. */
    XMP_OPEN_FORUPDATE = 0x00000002,       /**< Open for reading and writing. */
    XMP_OPEN_ONLYXMP = 0x00000004,         /**< Only the XMP is wanted,
                                            * allows space/time optimizations. */
    XMP_OPEN_CACHETNAIL = 0x00000008,      /**< Cache thumbnail if possible,
                                            * GetThumbnail will be called. */
    XMP_OPEN_STRICTLY = 0x00000010,        /**< Be strict about locating XMP
                                            * and reconciling with other forms. */
    XMP_OPEN_USESMARTHANDLER = 0x00000020, /**< Require the use of a smart
                                            * handler. */
    XMP_OPEN_USEPACKETSCANNING = 0x00000040, /**< Force packet scanning,
                                              * don't use a smart handler. */
    XMP_OPEN_LIMITSCANNING = 0x00000080,     /**< Only packet scan files "known"
                                              * to need scanning. */
    XMP_OPEN_REPAIR_FILE =
        0x00000100, /**< Attempt to repair a file opened for update,
                     * default is to not open (throw an exception). */
    XMP_OPEN_OPTIMIZEFILELAYOUT =
        0x00000200, /**< Optimize MPEG4 to support stream when updating
                     * This can take some time */
    XMP_OPEN_INBACKGROUND = 0x10000000 /**< Set if calling from background
                                        * thread. */
} XmpOpenFileOptions;

/** Option bits for xmp_files_close() */
typedef enum {
    XMP_CLOSE_NOOPTION = 0x0000,  /**< No close option */
    XMP_CLOSE_SAFEUPDATE = 0x0001 /**< Write into a temporary file and
                                   * swap for crash safety. */
} XmpCloseFileOptions;

typedef enum {

    /* Public file formats. Hex used to avoid gcc warnings. */
    /* ! Leave them as big endian. There seems to be no decent way on UNIX to
       determine the target */
    /* ! endianness at compile time. Forcing it on the client isn't acceptable.
       */

    XMP_FT_PDF = 0x50444620UL, /* 'PDF ' */
    XMP_FT_PS = 0x50532020UL,  /* 'PS  ', general PostScript following DSC
                                  conventions. */
    XMP_FT_EPS = 0x45505320UL, /* 'EPS ', encapsulated PostScript. */

    XMP_FT_JPEG = 0x4A504547UL,   /* 'JPEG' */
    XMP_FT_JPEG2K = 0x4A505820UL, /* 'JPX ', ISO 15444-1 */
    XMP_FT_TIFF = 0x54494646UL,   /* 'TIFF' */
    XMP_FT_GIF = 0x47494620UL,    /* 'GIF ' */
    XMP_FT_PNG = 0x504E4720UL,    /* 'PNG ' */
    XMP_FT_WEBP = 0x57454250UL,   /* 'WEBP' */

    XMP_FT_SWF = 0x53574620UL, /* 'SWF ' */
    XMP_FT_FLA = 0x464C4120UL, /* 'FLA ' */
    XMP_FT_FLV = 0x464C5620UL, /* 'FLV ' */

    XMP_FT_MOV = 0x4D4F5620UL,   /* 'MOV ', Quicktime */
    XMP_FT_AVI = 0x41564920UL,   /* 'AVI ' */
    XMP_FT_CIN = 0x43494E20UL,   /* 'CIN ', Cineon */
    XMP_FT_WAV = 0x57415620UL,   /* 'WAV ' */
    XMP_FT_MP3 = 0x4D503320UL,   /* 'MP3 ' */
    XMP_FT_SES = 0x53455320UL,   /* 'SES ', Audition session */
    XMP_FT_CEL = 0x43454C20UL,   /* 'CEL ', Audition loop */
    XMP_FT_MPEG = 0x4D504547UL,  /* 'MPEG' */
    XMP_FT_MPEG2 = 0x4D503220UL, /* 'MP2 ' */
    XMP_FT_MPEG4 = 0x4D503420UL, /* 'MP4 ', ISO 14494-12 and -14 */
    XMP_FT_MXF = 0x4D584620UL,   /* 'MXF ' */
    XMP_FT_WMAV = 0x574D4156UL,  /* 'WMAV', Windows Media Audio and Video */
    XMP_FT_AIFF = 0x41494646UL,  /* 'AIFF' */
    XMP_FT_RED = 0x52454420UL,   /* 'RED ', RED file format */
    XMP_FT_ARRI = 0x41525249UL,  /* 'ARRI', ARRI file format */
    XMP_FT_HEIF = 0x48454946UL,  /* 'HEIF', HEIF file format */
    XMP_FT_P2 = 0x50322020UL,    /* 'P2  ' */
    XMP_FT_XDCAM_FAM = 0x58444346UL, /* 'XDCF' */
    XMP_FT_XDCAM_SAM = 0x58444353UL, /* 'XDCS' */
    XMP_FT_XDCAM_EX = 0x58444358UL,  /* 'XDCX' */
    XMP_FT_AVCHD = 0x41564844UL,     /* 'AVHD' */
    XMP_FT_SONY_HDV = 0x53484456UL,  /* 'SHDV' */
    XMP_FT_CANON_XF = 0x434E5846UL,  /* 'CNXF' */
    XMP_FT_AVC_ULTRA = 0x41564355UL, /* 'AVCU' */

    XMP_FT_HTML = 0x48544D4CUL, /* 'HTML' */
    XMP_FT_XML = 0x584D4C20UL,  /* 'XML ' */
    XMP_FT_TEXT = 0x74657874UL, /* 'text' */
    XMP_FT_SVG = 0x53564720UL,  /* 'SVG ' */

    /* Adobe application file formats. */
    XMP_FT_PHOTOSHOP = 0x50534420UL,   /* 'PSD ' */
    XMP_FT_ILLUSTRATOR = 0x41492020UL, /* 'AI  ' */
    XMP_FT_INDESIGN = 0x494E4444UL,    /* 'INDD' */
    XMP_FT_AEPROJECT = 0x41455020UL,   /* 'AEP ' */
    XMP_FT_AEPROJTEMPLATE =
        0x41455420UL, /* 'AET ', After Effects Project Template */
    XMP_FT_AEFILTERPRESET = 0x46465820UL,  /* 'FFX ' */
    XMP_FT_ENCOREPROJECT = 0x4E434F52UL,   /* 'NCOR' */
    XMP_FT_PREMIEREPROJECT = 0x5052504AUL, /* 'PRPJ' */
    XMP_FT_PREMIERETITLE = 0x5052544CUL,   /* 'PRTL' */
    XMP_FT_UCF = 0x55434620UL,             /* 'UCF ', Universal Container Format */

    /* Catch all. */
    XMP_FT_UNKNOWN = 0x20202020UL /* '    ' */
} XmpFileType;

typedef enum {

    XMP_FMT_CAN_INJECT_XMP = 0x00000001,
    XMP_FMT_CAN_EXPAND = 0x00000002,
    XMP_FMT_CAN_REWRITE = 0x00000004,
    XMP_FMT_PREFERS_IN_PLACE = 0x00000008,
    XMP_FMT_CAN_RECONCILE = 0x00000010,
    XMP_FMT_ALLOWS_ONLY_XMP = 0x00000020,
    XMP_FMT_RETURNS_RAW_PACKET = 0x00000040,
    XMP_FMT_HANDLER_OWNS_FILE = 0x00000100,
    XMP_FMT_ALLOW_SAFE_UPDATE = 0x00000200,
    XMP_FMT_NEEDS_READONLY_PACKET = 0x00000400,
    XMP_FMT_USE_SIDECAR_XMP = 0x00000800,
    XMP_FMT_FOLDER_BASED_FORMAT = 0x00001000,

    _XMP_FMT_LAST
} XmpFileFormatOptions;

typedef enum {
    XMP_ITER_CLASSMASK = 0x00FFUL,  /**< The low 8 bits are an enum of
                                     * what data structure to iterate. */
    XMP_ITER_PROPERTIES = 0x0000UL, /**< Iterate the property tree of
                                     * a TXMPMeta object. */
    XMP_ITER_ALIASES = 0x0001UL,    /**< Iterate the global alias table. */
    XMP_ITER_NAMESPACES = 0x0002UL, /**< Iterate the global namespace table. */
    XMP_ITER_JUSTCHILDREN = 0x0100UL,   /**< Just do the immediate children
                                         * of the root, default is subtree. */
    XMP_ITER_JUSTLEAFNODES = 0x0200UL,  /**< Just do the leaf nodes, default
                                         * is all nodes in the subtree. */
    XMP_ITER_JUSTLEAFNAME = 0x0400UL,   /**< Return just the leaf part of the
                                         * path, default is the full path. */
    XMP_ITER_INCLUDEALIASES = 0x0800UL, /**< Include aliases, default is just
                                         * actual properties. */
    XMP_ITER_OMITQUALIFIERS = 0x1000UL  /* Omit all qualifiers. */
} XmpIterOptions;

typedef enum {
    XMP_ITER_SKIPSUBTREE = 0x0001UL, /**< Skip the subtree below the
                                      * current node. */
    XMP_ITER_SKIPSIBLINGS =
        0x0002UL /**< Skip the subtree below and remaining
                                                      * siblings of the current
                  * node. */
} XmpIterSkipOptions;

typedef enum {
    /** Options relating to the XML string form of the property value. */
    XMP_PROP_VALUE_IS_URI = 0x00000002UL, /**< The value is a URI, use
                                           * rdf:resource attribute.
                                           * DISCOURAGED */
    /** Options relating to qualifiers attached to a property. */
    XMP_PROP_HAS_QUALIFIERS = 0x00000010UL, /**< The property has qualifiers,
                                             * includes rdf:type and
                                             * xml:lang. */
    XMP_PROP_IS_QUALIFIER = 0x00000020UL,   /**< This is a qualifier,
                                             * includes rdf:type and
                                             * xml:lang. */
    XMP_PROP_HAS_LANG = 0x00000040UL,       /**< Implies XMP_PropHasQualifiers,
                                             * property has xml:lang. */
    XMP_PROP_HAS_TYPE = 0x00000080UL,       /**< Implies XMP_PropHasQualifiers,
                                             * property has rdf:type. */

    /* Options relating to the data structure form. */
    XMP_PROP_VALUE_IS_STRUCT = 0x00000100UL, /**< The value is a structure
                                              * with nested fields. */
    XMP_PROP_VALUE_IS_ARRAY = 0x00000200UL,  /**< The value is an array
                                              * (RDF alt/bag/seq). */
    XMP_PROP_ARRAY_IS_UNORDERED = XMP_PROP_VALUE_IS_ARRAY, /**< The item order
                                                            * does not matter.*/
    XMP_PROP_ARRAY_IS_ORDERED = 0x00000400UL, /**< Implies XMP_PropValueIsArray,
                                               * item order matters. */
    XMP_PROP_ARRAY_IS_ALT = 0x00000800UL, /**< Implies XMP_PropArrayIsOrdered,
                                           * items are alternates. */

    /** Additional struct and array options. */
    XMP_PROP_ARRAY_IS_ALTTEXT =
        0x00001000UL, /**< Implies kXMP_PropArrayIsAlternate,
                       * items are localized text. */
    XMP_PROP_ARRAY_INSERT_BEFORE =
        0x00004000UL,                           /**< Used by array functions. */
    XMP_PROP_ARRAY_INSERT_AFTER = 0x00008000UL, /**< Used by array functions. */

    /* Other miscellaneous options. */
    XMP_PROP_IS_ALIAS = 0x00010000UL,    /**< This property is an alias name for
                                            another property. */
    XMP_PROP_HAS_ALIASES = 0x00020000UL, /**< This property is the base value
                                            for a set of aliases. */
    XMP_PROP_IS_INTERNAL = 0x00040000UL, /**< This property is an "internal"
                                            property, owned by applications. */
    XMP_PROP_IS_STABLE = 0x00100000UL,   /**< This property is not derived from
                                          * the document content.
                                            */
    XMP_PROP_IS_DERIVED = 0x00200000UL,  /**< This property is derived from the
                                            document content. */
    /* kXMPUtil_AllowCommas   = 0x10000000UL,  ! Used by
       TXMPUtils::CatenateArrayItems and ::SeparateArrayItems. */
    /* kXMP_DeleteExisting    = 0x20000000UL,  ! Used by TXMPMeta::SetXyz
       functions to delete any pre-existing property. */
    /* kXMP_SchemaNode        = 0x80000000UL,  ! Returned by iterators - #define
       to avoid warnings */

    /* Masks that are multiple flags. */
    XMP_PROP_ARRAY_FORM_MASK = XMP_PROP_VALUE_IS_ARRAY |
                               XMP_PROP_ARRAY_IS_ORDERED |
                               XMP_PROP_ARRAY_IS_ALT |
                               XMP_PROP_ARRAY_IS_ALTTEXT,
    XMP_PROP_COMPOSITE_MASK = XMP_PROP_VALUE_IS_STRUCT |
                              XMP_PROP_ARRAY_FORM_MASK, /* Is it simple or
                                                           composite (array or
                                                           struct)? */
    XMP_IMPL_RESERVED_MASK =
        0x70000000L /**< Reserved for transient use by the implementation. */
} XmpPropsBits;

/* convenience macros */

/** set option bits */
#define XMP_SET_OPTION(var, opt) var |= (opt)
/** clear option bits */
#define XMP_CLEAR_OPTION(var, opt) var &= ~(opt)
/** test if option is set */
#define XMP_TEST_OPTION_SET(var, opt) (((var) & (opt)) != 0)
/** test if option is clear */
#define XMP_TEST_OPTION_CLEAR(var, opt) (((var) & (opt)) == 0)

#define XMP_IS_PROP_SIMPLE(opt) (((opt)&XMP_PROP_COMPOSITE_MASK) == 0)
#define XMP_IS_PROP_STRUCT(opt) (((opt)&XMP_PROP_VALUE_IS_STRUCT) != 0)
#define XMP_IS_PROP_ARRAY(opt) (((opt)&XMP_PROP_VALUE_IS_ARRAY) != 0)
#define XMP_IS_ARRAY_UNORDERED(opt) (((opt)&XMP_PROP_ARRAY_IS_ORDERED) == 0)
#define XMP_IS_ARRAY_ORDERED(opt) (((opt)&XMP_PROP_ARRAY_IS_ORDERED) != 0)
#define XMP_IS_ARRAY_ALT(opt) (((opt)&XMP_PROP_ARRAY_IS_ALT) != 0)
#define XMP_IS_ARRAY_ALTTEXT(opt) (((opt)&XMP_PROP_ARRAY_IS_ALTTEXT) != 0)

#define XMP_HAS_PROP_QUALIFIERS(opt) (((opt)&XMP_PROP_HAS_QUALIFIERS) != 0)
#define XMP_IS_PROP_QUALIFIER(opt) (((opt)&XMP_PROP_IS_QUALIFIER) != 0)
#define XMP_HAS_PROP_LANG(opt) (((opt)&XMP_PROP_HAS_LANG) != 0)

#define XMP_IS_NODE_SCHEMA(opt) (((opt)&XMP_SCHEMA_NODE) != 0)
#define XMP_IS_PROP_ALIAS(opt) (((opt)&XMP_PROP_IS_ALIAS) != 0)

enum {                                          /* Options for xmp_serialize */
       XMP_SERIAL_OMITPACKETWRAPPER = 0x0010UL, /**< Omit the XML packet
                                                 * wrapper. */
       XMP_SERIAL_READONLYPACKET = 0x0020UL,    /**< Default is a writeable
                                                 * packet. */
       XMP_SERIAL_USECOMPACTFORMAT = 0x0040UL,  /**< Use a compact form of
                                                 * RDF. */

       XMP_SERIAL_INCLUDETHUMBNAILPAD =
           0x0100UL,                            /**< Include a padding allowance
                                                 * for a thumbnail image. */
       XMP_SERIAL_EXACTPACKETLENGTH = 0x0200UL, /**< The padding parameter is
                                                 * the overall packet length. */
       XMP_SERIAL_WRITEALIASCOMMENTS = 0x0400UL, /**< Show aliases as XML
                                                  * comments. */
       XMP_SERIAL_OMITALLFORMATTING = 0x0800UL,  /**< Omit all formatting
                                                  * whitespace. */

       _XMP_LITTLEENDIAN_BIT =
           0x0001UL, /* ! Don't use directly, see the combined values below! */
       _XMP_UTF16_BIT = 0x0002UL,
       _XMP_UTF32_BIT = 0x0004UL,

       XMP_SERIAL_ENCODINGMASK = 0x0007UL,
       XMP_SERIAL_ENCODEUTF8 = 0UL,
       XMP_SERIAL_ENCODEUTF16BIG = _XMP_UTF16_BIT,
       XMP_SERIAL_ENCODEUTF16LITTLE = _XMP_UTF16_BIT | _XMP_LITTLEENDIAN_BIT,
       XMP_SERIAL_ENCODEUTF32BIG = _XMP_UTF32_BIT,
       XMP_SERIAL_ENCODEUTF32LITTLE = _XMP_UTF32_BIT | _XMP_LITTLEENDIAN_BIT
};

/** pointer to XMP packet. Opaque. */
typedef struct _Xmp *XmpPtr;
typedef struct _XmpFile *XmpFilePtr;
typedef struct _XmpString *XmpStringPtr;
typedef struct _XmpIterator *XmpIteratorPtr;

typedef struct _XmpDateTime {
    int32_t year;
    int32_t month;    /* 1..12 */
    int32_t day;      /* 1..31 */
    int32_t hour;     /* 0..23 */
    int32_t minute;   /* 0..59 */
    int32_t second;   /* 0..59 */
    int32_t tzSign;   /* -1..+1, 0 means UTC, -1 is west, +1 is east. */
    int32_t tzHour;   /* 0..23 */
    int32_t tzMinute; /* 0..59 */
    int32_t nanoSecond;
} XmpDateTime;

typedef struct _XmpPacketInfo {
  /// Packet offset in the file in bytes, -1 if unknown.
  int64_t offset;
  /// Packet length in the file in bytes, -1 if unknown.
  int32_t length;
  /// Packet padding size in bytes, zero if unknown.
  int32_t padSize;

  /// Character format using the values \c kXMP_Char8Bit,
  /// \c kXMP_Char16BitBig, etc.
  uint8_t  charForm;
  /// True if there is a packet wrapper and the trailer says writeable
  /// by dumb packet scanners.
  bool  writeable;
  /// True if there is a packet wrapper, the "<?xpacket...>"
  /// XML processing instructions.
  bool  hasWrapper;

  /// Padding to make the struct's size be a multiple 4.
  uint8_t  pad;
} XmpPacketInfo;

/** Values used for tzSign field. */
enum {
    XMP_TZ_WEST = -1, /**< West of UTC   */
    XMP_TZ_UTC = 0,   /**< UTC           */
    XMP_TZ_EAST = +1  /**< East of UTC   */
};

/** Init the library. Must be called before anything else */
bool xmp_init(void);
void xmp_terminate(void);

/** get the error code that last occurred.
 * @todo make this thread-safe. Getting the error code
 * from another thread than the one it occurred on is undefined.
 */
int xmp_get_error(void);

XmpFilePtr xmp_files_new(void);

/** Open a file to load the XMP from.
 * @param path the file path
 * @param options open flags
 * @return an XmpFilePtr if successful.
 */
XmpFilePtr xmp_files_open_new(const char *path, XmpOpenFileOptions options);

/** Open a file to load the XMP from into an existing XmpFile.
 * See %xmp_file_open_new
 * @paran xf the XmpFilePtr to load into.
 * @param path the file path
 * @param options open flags
 * @return true if successful.
 */
bool xmp_files_open(XmpFilePtr xf, const char *path, XmpOpenFileOptions options);

/** Close an XMP file. Will flush the changes
 * @param xf the file object
 * @param options the options to close.
 * @return true on succes, false on error
 * xmp_get_error() will give the error code.
 */
bool xmp_files_close(XmpFilePtr xf, XmpCloseFileOptions options);

/** Get the XMP packet from the file
 * If the file has a handler, the handler will be used and reconcile depending
 * on the options. Otherwise it will try to locate the XMP packet wrapper.
 * Note: a XMP sidecar shouldn't have a pacet wrapper, therefor it shouldn't
 * recognized by this.
 * @param xf the %XmpFilePtr to get the XMP packet from
 * @return a newly allocated XmpPtr. Use %xmp_free to release it.
 */
XmpPtr xmp_files_get_new_xmp(XmpFilePtr xf);

/** File the XMP packet from the file
 * See %xmp_files_get_new_xmp
 * @param xf the %XmpFilePtr to get the XMP packet from
 * @param xmp the XMP Packet to fill. Must be valid.
 */
bool xmp_files_get_xmp(XmpFilePtr xf, XmpPtr xmp);
bool xmp_files_get_xmp_xmpstring(XmpFilePtr xf, XmpStringPtr xmp_packet,
                                 XmpPacketInfo* packet_info);

bool xmp_files_can_put_xmp(XmpFilePtr xf, XmpPtr xmp);
bool xmp_files_can_put_xmp_xmpstring(XmpFilePtr xf, XmpStringPtr xmp_packet);
bool xmp_files_can_put_xmp_cstr(XmpFilePtr xf, const char* xmp_packet,
                                size_t len);

bool xmp_files_put_xmp(XmpFilePtr xf, XmpPtr xmp);
bool xmp_files_put_xmp_xmpstring(XmpFilePtr xf, XmpStringPtr xmp_packet);
bool xmp_files_put_xmp_cstr(XmpFilePtr xf, const char* xmp_packet, size_t len);

/** Get the file info from the open file
 * @param xf the file object
 * @param[out] filePath the file path object to store the path in. Pass NULL if
 * not needed.
 * @param[out] options the options for open. Pass NULL if not needed.
 * @param[out] file_format the detected file format. Pass NULL if not needed.
 * @param[out] handler_flags the format options like from
 * %xmp_files_get_format_info.
 * @return false in case of error.
 */
bool xmp_files_get_file_info(XmpFilePtr xf, XmpStringPtr filePath,
                             XmpOpenFileOptions *options,
                             XmpFileType *file_format,
                             XmpFileFormatOptions *handler_flags);

/** Free a XmpFilePtr
 * @param xf the file ptr. Cannot be NULL
 * @return false on error.
 * Calll %xmp_get_error to retrieve the error code.
 */
bool xmp_files_free(XmpFilePtr xf);

/** Get the format info
 * @param format type identifier
 * @param options the options for the file format handler
 * @return false on error
 */
bool xmp_files_get_format_info(XmpFileType format,
                               XmpFileFormatOptions *options);

/** Check the file format of a file. Use the same logic as in xmp_files_open().
 * A file that doesn't have a handler will be considered as unknown. This doesn't
 * mean that xmp_files_get_new_xmp() will fail.
 * @param filePath the path to the file
 * @return XMP_FT_UNKNOWN on error or if the file type is unknown
 */
XmpFileType xmp_files_check_file_format(const char *filePath);

/** Register a new namespace to add properties to
 *  This is done automatically when reading the metadata block
 *  @param namespaceURI the namespace URI to register
 *  @param suggestedPrefix the suggested prefix
 *  @param registeredPrefix the really registered prefix. Not necessarily
 *  %suggestedPrefix.
 *  @return true if success, false otherwise.
 */
bool xmp_register_namespace(const char *namespaceURI,
                            const char *suggestedPrefix,
                            XmpStringPtr registeredPrefix);

/** Check is a namespace is registered
 *  @param ns the namespace to check.
 *  @param prefix The prefix associated if registered. Pass NULL
 *  if not interested.
 *  @return true if registered.
 *  NEW in 2.1
 */
bool xmp_namespace_prefix(const char *ns, XmpStringPtr prefix);

/** Check if a ns prefix is registered.
 *  @param prefix the prefix to check.
 *  @param ns the namespace associated if registered. Pass NULL
 *  if not interested.
 *  @return true if registered.
 *  NEW in 2.1
 */
bool xmp_prefix_namespace_uri(const char *prefix, XmpStringPtr ns);

/** Create a new empty XMP packet
 * @return the packet pointer. Must be free with xmp_free()
 */
XmpPtr xmp_new_empty(void);

/** Create a new XMP packet
 * @param buffer the buffer to load data from. UTF-8 encoded.
 * @param len the buffer length in byte
 * @return the packet pointer. Must be free with xmp_free()
 */
XmpPtr xmp_new(const char *buffer, size_t len);

/** Create a new XMP packet from the one passed.
 * @param xmp the instance to copy. Can be NULL.
 * @return the packet pointer. NULL is failer (or NULL is passed).
 */
XmpPtr xmp_copy(XmpPtr xmp);

/** Free the xmp packet
 * @param xmp the xmp packet to free
 */
bool xmp_free(XmpPtr xmp);

/** Parse the XML passed through the buffer and load it.
 * @param xmp the XMP packet.
 * @param buffer the buffer.
 * @param len the length of the buffer.
 */
bool xmp_parse(XmpPtr xmp, const char *buffer, size_t len);

/** Serialize the XMP Packet to the given buffer
 * @param xmp the XMP Packet
 * @param buffer the buffer to write the XMP to
 * @param options options on how to write the XMP.  See XMP_SERIAL_*
 * @param padding number of bytes of padding, useful for modifying
 *                embedded XMP in place.
 * @return TRUE if success.
 */
bool xmp_serialize(XmpPtr xmp, XmpStringPtr buffer, uint32_t options,
                   uint32_t padding);

/** Serialize the XMP Packet to the given buffer with formatting
 * @param xmp the XMP Packet
 * @param buffer the buffer to write the XMP to
 * @param options options on how to write the XMP.  See XMP_SERIAL_*
 * @param padding number of bytes of padding, useful for modifying
 *                embedded XMP in place.
 * @param newline the new line character to use
 * @param tab the indentation character to use
 * @param indent the initial indentation level
 * @return TRUE if success.
 */
bool xmp_serialize_and_format(XmpPtr xmp, XmpStringPtr buffer, uint32_t options,
                              uint32_t padding, const char *newline,
                              const char *tab, int32_t indent);

/** Get an XMP property and it option bits from the XMP packet
 * @param xmp the XMP packet
 * @param schema
 * @param name
 * @param property the allocated XmpStringPtr
 * @param propsBits pointer to the option bits. Pass NULL if not needed
 * @return true if found
 */
bool xmp_get_property(XmpPtr xmp, const char *schema, const char *name,
                      XmpStringPtr property, uint32_t *propsBits);

bool xmp_get_property_date(XmpPtr xmp, const char *schema, const char *name,
                           XmpDateTime *property, uint32_t *propsBits);
bool xmp_get_property_float(XmpPtr xmp, const char *schema, const char *name,
                            double *property, uint32_t *propsBits);
bool xmp_get_property_bool(XmpPtr xmp, const char *schema, const char *name,
                           bool *property, uint32_t *propsBits);
bool xmp_get_property_int32(XmpPtr xmp, const char *schema, const char *name,
                            int32_t *property, uint32_t *propsBits);
bool xmp_get_property_int64(XmpPtr xmp, const char *schema, const char *name,
                            int64_t *property, uint32_t *propsBits);

/** Get an item frpm an array property
 * @param xmp the xmp meta
 * @param schema the schema
 * @param name the property name
 * @param index the index in the array
 * @param property the property value
 * @param propsBits the property bits. Pass NULL is unwanted.
 * @return TRUE if success.
 */
bool xmp_get_array_item(XmpPtr xmp, const char *schema, const char *name,
                        int32_t index, XmpStringPtr property,
                        uint32_t *propsBits);

/** Set an XMP property in the XMP packet
 * @param xmp the XMP packet
 * @param schema
 * @param name
 * @param value 0 terminated string
 * @param optionBits
 * @return false if failure
 */
bool xmp_set_property(XmpPtr xmp, const char *schema, const char *name,
                      const char *value, uint32_t optionBits);

/** Set a date XMP property in the XMP packet
 * @param xmp the XMP packet
 * @param schema
 * @param name
 * @param value the date-time struct
 * @param optionBits
 * @return false if failure
 */
bool xmp_set_property_date(XmpPtr xmp, const char *schema, const char *name,
                           const XmpDateTime *value, uint32_t optionBits);

/** Set a float XMP property in the XMP packet
 * @param xmp the XMP packet
 * @param schema
 * @param name
 * @param value the float value
 * @param optionBits
 * @return false if failure
 */
bool xmp_set_property_float(XmpPtr xmp, const char *schema, const char *name,
                            double value, uint32_t optionBits);
bool xmp_set_property_bool(XmpPtr xmp, const char *schema, const char *name,
                           bool value, uint32_t optionBits);
bool xmp_set_property_int32(XmpPtr xmp, const char *schema, const char *name,
                            int32_t value, uint32_t optionBits);
bool xmp_set_property_int64(XmpPtr xmp, const char *schema, const char *name,
                            int64_t value, uint32_t optionBits);

bool xmp_set_array_item(XmpPtr xmp, const char *schema, const char *name,
                        int32_t index, const char *value, uint32_t optionBits);

/** Append a value to the XMP Property array in the XMP Packet provided
 * @param xmp the XMP packet
 * @param schema the schema of the property
 * @param name the name of the property
 * @param arrayOptions option bits of the parent array
 * @param value null-terminated string
 * @param optionBits option bits of the value itself.
 */
bool xmp_append_array_item(XmpPtr xmp, const char *schema, const char *name,
                           uint32_t arrayOptions, const char *value,
                           uint32_t optionBits);

/** Delete a property from the XMP Packet provided
 * @param xmp the XMP packet
 * @param schema the schema of the property
 * @param name the name of the property
 */
bool xmp_delete_property(XmpPtr xmp, const char *schema, const char *name);

/** Determines if a property exists in the XMP Packet provided
 * @param xmp the XMP packet
 * @param schema the schema of the property. Can't be NULL or empty.
 * @param name the name of the property. Can't be NULL or empty.
 * @return true is the property exists
 */
bool xmp_has_property(XmpPtr xmp, const char *schema, const char *name);

/** Get a localised text from a localisable property.
 * @param xmp the XMP packet
 * @param schema the schema
 * @param name the property name.
 * @param genericLang the generic language you may want as a fall back.
 * Can be NULL or empty.
 * @param specificLang the specific language you want. Can't be NULL or empty.
 * @param actualLang the actual language of the value. Can be NULL if
 * not wanted.
 * @param itemValue the localized value. Can be NULL if not wanted.
 * @param propBits the options flags describing the property. Can be NULL.
 * @return true if found, false otherwise.
 */
bool xmp_get_localized_text(XmpPtr xmp, const char *schema, const char *name,
                            const char *genericLang, const char *specificLang,
                            XmpStringPtr actualLang, XmpStringPtr itemValue,
                            uint32_t *propBits);

/** Set a localised text in a localisable property.
 * @param xmp the XMP packet
 * @param schema the schema
 * @param name the property name.
 * @param genericLang the generic language you may want to set too.
 * Can be NULL or empty.
 * @param specificLang the specific language you want. Can't be NULL or empty.
 * @param value the localized value. Cannot be NULL.
 * @param optionBits the options flags describing the property.
 * @return true if set, false otherwise.
 */
bool xmp_set_localized_text(XmpPtr xmp, const char *schema, const char *name,
                            const char *genericLang, const char *specificLang,
                            const char *value, uint32_t optionBits);

bool xmp_delete_localized_text(XmpPtr xmp, const char *schema, const char *name,
                               const char *genericLang,
                               const char *specificLang);

/** Instanciate a new string
 * @return the new instance. Must be freed with
 * xmp_string_free()
 */
XmpStringPtr xmp_string_new(void);

/** Free a XmpStringPtr
 * @param s the string to free
 */
void xmp_string_free(XmpStringPtr s);

/** Get the C string from the XmpStringPtr
 * @param s the string object
 * @return the const char * for the XmpStringPtr. It
 * belong to the object.
 */
const char * xmp_string_cstr(XmpStringPtr s);

/** Get the string length from the XmpStringPtr
 * @param s the string object
 * @return the string length. The unerlying implementation has it.
 */
size_t xmp_string_len(XmpStringPtr s);

/** Create a new iterator.
 * @param xmp the packet
 * @param schema the property schema
 * @param propName the property name
 * @param options
 * @return an iterator must be freed with % xmp_iterator_free
 */
XmpIteratorPtr xmp_iterator_new(XmpPtr xmp, const char *schema,
                                const char *propName, XmpIterOptions options);

/** Free an iterator.
 * @param iter the iterator to free.
 */
bool xmp_iterator_free(XmpIteratorPtr iter);

/** Iterate to the next value
 * @param iter the iterator
 * @param schema the schema name. Pass NULL if not wanted
 * @param propName the property path. Pass NULL if not wanted
 * @param propValue the value of the property. Pass NULL if not wanted.
 * @param options the options for the property. Pass NULL if not wanted.
 * @return true if still something, false if none
 */
bool xmp_iterator_next(XmpIteratorPtr iter, XmpStringPtr schema,
                       XmpStringPtr propName, XmpStringPtr propValue,
                       uint32_t *options);

/**
 */
bool xmp_iterator_skip(XmpIteratorPtr iter, XmpIterSkipOptions options);

/** Compare two XmpDateTime
 * @param left value
 * @param right value
 * @return if left < right, return < 0. If left > right, return > 0.
 * if left == right, return 0.
 */
int xmp_datetime_compare(XmpDateTime *left, XmpDateTime *right);

#ifdef __cplusplus
}
#endif

#endif
