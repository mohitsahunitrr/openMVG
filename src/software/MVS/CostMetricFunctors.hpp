#ifndef _DEPTH_MAP_COMPUTATION_CPU_COST_METRIC_FUNCTOR_HH_
#define _DEPTH_MAP_COMPUTATION_CPU_COST_METRIC_FUNCTOR_HH_

#include "DepthMapComputationParameters.hpp"
#include "Image.hpp"

#include "daisy/daisy.h"

#include <map>

namespace MVS
{

/**
 * @brief Base class for Cost metric aggregation score
 */
class AbstractCostMetric
{
public:
  /**
     * @brief Ctr
     * @param img_ref Reference image
     * @param img_other Target image
     * @param params Various parameters of the functor
     */
  AbstractCostMetric( const Image& img_ref, const Image& img_other, const DepthMapComputationParameters& params );

  virtual ~AbstractCostMetric( void );

  /**
     * @brief Compute aggregated matching cost at a given pixel
     * @param id_row Row index of the queried pixel
     * @param id_col Col index of the queried pixel
     * @param H Homography that maps pixels from Reference image to the Target image
     * @return Aggregated matching cost at specified pixel
     */
  virtual double operator()( const int id_row, const int id_col, const openMVG::Mat3& H ) const = 0;

protected:
  const Image&                         m_image_ref;
  const Image&                         m_image_other;
  const DepthMapComputationParameters& m_params;
};

/**
 * @brief Zero-mean Normalized Cross Correlation cost metric
 */
class ZNCCCostMetric : public AbstractCostMetric
{
public:
  /**
     * @brief Ctr
     * @param img_ref Reference image
     * @param img_other Target image
     * @param params Various parameters of the functor
     */
  ZNCCCostMetric( const Image& img_ref, const Image& img_other, const DepthMapComputationParameters& params );

  /**
   * @brief Dtr
   * 
   */
  virtual ~ZNCCCostMetric( void );

  /**
     * @brief Compute aggregated matching cost at a given pixel
     * @param id_row Row index of the queried pixel
     * @param id_col Col index of the queried pixel
     * @param H Homography that maps pixels from Reference image to the Target image
     * @return Aggregated matching cost at specified pixel
     */
  double operator()( const int id_row, const int id_col, const openMVG::Mat3& H ) const override;
};

/**
 * Patch-Match cost metric
 */
class PatchMatchCostMetric : public AbstractCostMetric
{
public:
  /**
     * @brief Ctr
     * @param img_ref Reference image
     * @param img_other Target image
     * @param params Various parameters of the functor
     */
  PatchMatchCostMetric( const Image& img_ref, const Image& img_other, const DepthMapComputationParameters& params );

  /**
   * @brief Dtr
   * 
   */
  virtual ~PatchMatchCostMetric( void );

  /**
     * @brief Compute aggregated matching cost at a given pixel
     * @param id_row Row index of the queried pixel
     * @param id_col Col index of the queried pixel
     * @param H Homography that maps pixels from Reference image to the Target image
     * @return Aggregated matching cost at specified pixel
     */
  double operator()( const int id_row, const int id_col, const openMVG::Mat3& H ) const override;
};

/**
 * @brief Census cost metric
 */
class CensusCostMetric : public AbstractCostMetric
{
public:
  /**
     * @brief Ctr
     * @param img_ref Reference image
     * @param img_other Target image
     * @param params Various parameters of the functor
     */
  CensusCostMetric( const Image& img_ref, const Image& img_other, const DepthMapComputationParameters& params );

  virtual ~CensusCostMetric( void );

  /**
     * @brief Compute aggregated matching cost at a given pixel
     * @param id_row Row index of the queried pixel
     * @param id_col Col index of the queried pixel
     * @param H Homography that maps pixels from Reference image to the Target image
     * @return Aggregated matching cost at specified pixel
     */
  double operator()( const int id_row, const int id_col, const openMVG::Mat3& H ) const override;
};

/**
 * @brief DAISY based cost metric
 */
class DaisyCostMetric : public AbstractCostMetric
{
public:
  /**
     * @brief Ctr
     * @param img_ref Reference image
     * @param img_other Target image
     * @param params Various parameters of the functor
     */
  DaisyCostMetric( const Image& img_ref, const Image& img_other, const DepthMapComputationParameters& params );

  virtual ~DaisyCostMetric( void );

  /**
     * @brief Compute aggregated matching cost at a given pixel
     * @param id_row Row index of the queried pixel
     * @param id_col Col index of the queried pixel
     * @param H Homography that maps pixels from Reference image to the Target image
     * @return Aggregated matching cost at specified pixel
     */
  double operator()( const int id_row, const int id_col, const openMVG::Mat3& H ) const override;

  /**
     * @brief Release internal memory
     */
  static void releaseInternalMemory( void );

private:
  static std::map<Image, std::shared_ptr<daisy>> all_daisy_descs;

  std::shared_ptr<daisy> m_desc_ref;
  std::shared_ptr<daisy> m_desc_other;
};

class BilateralWeightedNCC : public AbstractCostMetric
{
public:

  /**
    * @brief Ctr
    * @param img_ref Reference image
    * @param img_other Target image
    * @param params Various parameters of the functor
    */
  BilateralWeightedNCC( const Image& img_ref, const Image& img_other, const DepthMapComputationParameters& params );

  virtual ~BilateralWeightedNCC( void );

  /**
    * @brief Compute aggregated matching cost at a given pixel
    * @param id_row Row index of the queried pixel
    * @param id_col Col index of the queried pixel
    * @param H Homography that maps pixels from Reference image to the Target image
    * @return Aggregated matching cost at specified pixel
    */
  double operator()( const int id_row, const int id_col, const openMVG::Mat3& H ) const override;

private:
  double weight( const int delta_row, const int delta_col, const int delta_color ) const;

  double m_inv_sigma_color_2;
  double m_inv_sigma_distance_2;

  int m_window;
  int m_half_window;
  int m_step;

  openMVG::image::Image<double> m_inv_sum_weights;
  openMVG::image::Image<double> m_sum_w_ref;
  openMVG::image::Image<double> m_variance_w_ref;

  static std::vector<double> m_precomputed_spatial_weight;
  static std::vector<double> m_precomputed_color_weight;
};

} // namespace MVS

#endif