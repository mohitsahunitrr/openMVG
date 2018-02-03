// This file is part of OpenMVG, an Open Multiple View Geometry C++ library.

// Copyright (c) 2015 Pierre MOULON.
// Copyright (c) 2018 Romuald PERROT.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/*

== Patent Warning and License =================================================

The SIFT method is patented

    [2] "Method and apparatus for identifying scale invariant features
      in an image."
        David G. Lowe
        Patent number: 6711293
        Filing date: Mar 6, 2000
        Issue date: Mar 23, 2004
        Application number: 09/519,89

 These source codes are made available for the exclusive aim of serving as
 scientific tool to verify the soundness and completeness of the algorithm
 description. Compilation, execution and redistribution of this file may
 violate patents rights in certain countries. The situation being different
 for every country and changing over time, it is your responsibility to
 determine which patent rights restrictions apply to you before you compile,
 use, modify, or redistribute this file. A patent lawyer is qualified to make
 this determination. If and only if they don't conflict with any patent terms,
 you can benefit from the following license terms attached to this file.

The implementation is based on

    [1] "Anatomy of the SIFT Method."
        I. Rey Otero  and  M. Delbracio
        Image Processing Online, 2013.
        http://www.ipol.im/pub/algo/rd_anatomy_sift/

Changes are:
- The hierarchical scale space code can be run on its own,
- Keypoint detection and description is split in two separate modules,
- the code can run per Octave (less memory consuming),
- some computation can be run in parallel.

*/

#ifndef OPENMVG_FEATURES_SIFT_SIFT_ANATOMY_IMAGE_DESCRIBER_GPU_HPP
#define OPENMVG_FEATURES_SIFT_SIFT_ANATOMY_IMAGE_DESCRIBER_GPU_HPP

#include <iostream>
#include <numeric>
#include <vector>

#include "openMVG/features/feature.hpp"
#include "openMVG/features/image_describer.hpp"
#include "openMVG/features/regions_factory.hpp"


namespace openMVG
{
namespace features
{
namespace gpu
{

class SIFT_Anatomy_Image_describerGPU : public Image_describer
{
  public:

    using Regions_type = SIFT_Regions;

    struct Params
    {
      Params(
        int first_octave = 0,
        int num_octaves = 6,
        int num_scales = 3,
        float edge_threshold = 10.0f,
        float peak_threshold = 0.04f,
        bool root_sift = true
      ) ;

      template<class Archive>
      inline void serialize( Archive & ar );

      // Parameters
      int first_octave_;      // Use original image, or perform an upscale if == -1
      int num_octaves_;       // Max octaves count
      int num_scales_;        // Scales per octave
      float edge_threshold_;  // Max ratio of Hessian eigenvalues
      float peak_threshold_;  // Min contrast
      bool root_sift_;        // see [1]
    };

    explicit SIFT_Anatomy_Image_describerGPU( const Params & params = Params() ) ;


    bool Set_configuration_preset( EDESCRIBER_PRESET preset ) override ;


    /**
    @brief Detect regions on the image and compute their attributes (description)
    @param image Image.
    @param mask 8-bit gray image for keypoint filtering (optional).
       Non-zero values depict the region of interest.
    @return regions The detected regions and attributes (the caller must delete the allocated data)
    */
    std::unique_ptr<Regions_type> Describe_SIFT_AnatomyGPU(
      const image::Image<unsigned char>& image,
      const image::Image<unsigned char>* mask = nullptr
    ) ;

    std::unique_ptr<Regions> Allocate() const override ;

    template<class Archive>
    inline void serialize( Archive & ar );

    std::unique_ptr<Regions> Describe(
      const image::Image<unsigned char>& image,
      const image::Image<unsigned char>* mask = nullptr ) override ;

  private:
    Params params_;
};

} // namespace gpu
} // namespace features
} // namespace openMVG


#endif // OPENMVG_FEATURES_SIFT_SIFT_ANATOMY_IMAGE_DESCRIBER_GPU_HPP
