// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_VIDEO_COLOR_SPACE_H_
#define MEDIA_BASE_VIDEO_COLOR_SPACE_H_

#include "media/base/media_export.h"
//#include "ui/gfx/color_space.h"
#include <stdint.h>
namespace gfx {
    class  ColorSpace {
    public:
        enum class PrimaryID : uint8_t {
            INVALID,
            BT709,
            BT470M,
            BT470BG,
            SMPTE170M,
            SMPTE240M,
            FILM,
            BT2020,
            SMPTEST428_1,
            SMPTEST431_2,
            SMPTEST432_1,
            XYZ_D50,
            ADOBE_RGB,
            // Corresponds the the primaries of the "Generic RGB" profile used in the
            // Apple ColorSync application, used by layout tests on Mac.
            APPLE_GENERIC_RGB,
            // A very wide gamut space with rotated primaries. Used by layout tests.
            WIDE_GAMUT_COLOR_SPIN,
            // Primaries defined by the primary matrix |custom_primary_matrix_|.
            CUSTOM,
            kMaxValue = CUSTOM,
        };

        enum class TransferID : uint8_t {
            INVALID,
            BT709,
            // On macOS, BT709 hardware decoded video frames, when displayed as
            // overlays, will have a transfer function of gamma=1.961.
            BT709_APPLE,
            GAMMA18,
            GAMMA22,
            GAMMA24,
            GAMMA28,
            SMPTE170M,
            SMPTE240M,
            LINEAR,
            LOG,
            LOG_SQRT,
            IEC61966_2_4,
            BT1361_ECG,
            IEC61966_2_1,
            BT2020_10,
            BT2020_12,
            SMPTEST2084,
            SMPTEST428_1,
            ARIB_STD_B67,  // AKA hybrid-log gamma, HLG.
            // The same as IEC61966_2_1 on the interval [0, 1], with the nonlinear
            // segment continuing beyond 1 and point symmetry defining values below 0.
            IEC61966_2_1_HDR,
            // The same as LINEAR but is defined for all real values.
            LINEAR_HDR,
            // A parametric transfer function defined by |transfer_params_|.
            CUSTOM,
            // An HDR parametric transfer function defined by |transfer_params_|.
            CUSTOM_HDR,
            // An HDR transfer function that is piecewise sRGB, and piecewise linear.
            PIECEWISE_HDR,
            kMaxValue = PIECEWISE_HDR,
        };

        enum class MatrixID : uint8_t {
            INVALID,
            RGB,
            BT709,
            FCC,
            BT470BG,
            SMPTE170M,
            SMPTE240M,
            YCOCG,
            BT2020_NCL,
            BT2020_CL,
            YDZDX,
            GBR,
            kMaxValue = GBR,
        };

        enum class RangeID : uint8_t {
            INVALID,
            // Limited Rec. 709 color range with RGB values ranging from 16 to 235.
            LIMITED,
            // Full RGB color range with RGB valees from 0 to 255.
            FULL,
            // Range is defined by TransferID/MatrixID.
            DERIVED,
            kMaxValue = DERIVED,
        };

        constexpr ColorSpace() {}
        constexpr ColorSpace(PrimaryID primaries, TransferID transfer)
            /*: ColorSpace(primaries, transfer, MatrixID::RGB, RangeID::FULL) */ {}
        /*constexpr ColorSpace(PrimaryID primaries,
            TransferID transfer,
            MatrixID matrix,
            RangeID range)
            : primaries_(primaries),
            transfer_(transfer),
            matrix_(matrix),
            range_(range) {}
        ColorSpace(PrimaryID primaries,
            TransferID transfer,
            MatrixID matrix,
            RangeID range,
            const skcms_Matrix3x3* custom_primary_matrix,
            const skcms_TransferFunction* cunstom_transfer_fn);*/

            //explicit ColorSpace(const SkColorSpace& sk_color_space);

            // Returns true if this is not the default-constructor object.
        bool IsValid() const;

        /* static constexpr ColorSpace CreateSRGB() {
             return ColorSpace(PrimaryID::BT709, TransferID::IEC61966_2_1, MatrixID::RGB,
                 RangeID::FULL);
         }

         static constexpr ColorSpace CreateDisplayP3D65() {
             return ColorSpace(PrimaryID::SMPTEST432_1, TransferID::IEC61966_2_1,
                 MatrixID::RGB, RangeID::FULL);
         }*/
         /*     static ColorSpace CreateCustom(const skcms_Matrix3x3& to_XYZD50,
                  const skcms_TransferFunction& fn);
              static ColorSpace CreateCustom(const skcms_Matrix3x3& to_XYZD50,
                  TransferID transfer);*/
                  /*static constexpr ColorSpace CreateXYZD50() {
                      return ColorSpace(PrimaryID::XYZ_D50, TransferID::LINEAR, MatrixID::RGB,
                          RangeID::FULL);
                  }*/
    };
} // namespace gfx

namespace media {



// Described in ISO 23001-8:2016
class MEDIA_EXPORT VideoColorSpace {
 public:
  // Table 2
  enum class PrimaryID : uint8_t {
    INVALID = 0,
    BT709 = 1,
    UNSPECIFIED = 2,
    BT470M = 4,
    BT470BG = 5,
    SMPTE170M = 6,
    SMPTE240M = 7,
    FILM = 8,
    BT2020 = 9,
    SMPTEST428_1 = 10,
    SMPTEST431_2 = 11,
    SMPTEST432_1 = 12,
    EBU_3213_E = 22
  };

  // Table 3
  enum class TransferID : uint8_t {
    INVALID = 0,
    BT709 = 1,
    UNSPECIFIED = 2,
    GAMMA22 = 4,
    GAMMA28 = 5,
    SMPTE170M = 6,
    SMPTE240M = 7,
    LINEAR = 8,
    LOG = 9,
    LOG_SQRT = 10,
    IEC61966_2_4 = 11,
    BT1361_ECG = 12,
    IEC61966_2_1 = 13,
    BT2020_10 = 14,
    BT2020_12 = 15,
    SMPTEST2084 = 16,
    SMPTEST428_1 = 17,

    // Not yet standardized
    ARIB_STD_B67 = 18,  // AKA hybrid-log gamma, HLG.
  };

  // Table 4
  enum class MatrixID : uint8_t {
    RGB = 0,
    BT709 = 1,
    UNSPECIFIED = 2,
    FCC = 4,
    BT470BG = 5,
    SMPTE170M = 6,
    SMPTE240M = 7,
    YCOCG = 8,
    BT2020_NCL = 9,
    BT2020_CL = 10,
    YDZDX = 11,
    INVALID = 255,
  };

  VideoColorSpace();
  VideoColorSpace(int primaries,
                  int transfer,
                  int matrix,
                  gfx::ColorSpace::RangeID range);
  VideoColorSpace(PrimaryID primaries,
                  TransferID transfer,
                  MatrixID matrix,
                  gfx::ColorSpace::RangeID range);

  bool operator==(const VideoColorSpace& other) const;
  bool operator!=(const VideoColorSpace& other) const;
  // Returns true if any of the fields have a value other
  // than INVALID or UNSPECIFIED.
  bool IsSpecified() const;

  // These will return INVALID if the number you give it
  // is not a valid enum value.
  static PrimaryID GetPrimaryID(int primary);
  static TransferID GetTransferID(int transfer);
  static MatrixID GetMatrixID(int matrix);

  static VideoColorSpace REC709();
  static VideoColorSpace REC601();
  static VideoColorSpace JPEG();

  gfx::ColorSpace ToGfxColorSpace() const;

  // Note, these are public variables.
  PrimaryID primaries = PrimaryID::INVALID;
  TransferID transfer = TransferID::INVALID;
  MatrixID matrix = MatrixID::INVALID;
  gfx::ColorSpace::RangeID range = gfx::ColorSpace::RangeID::INVALID;
};

}  // namespace media

#endif
