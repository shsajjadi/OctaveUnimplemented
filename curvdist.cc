// Copyright (C) 2020 Seyyed Hossein Sajjadi
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include <algorithm>
#include <limits>
#include <queue>

#include <octave/oct.h>

namespace image
{

  enum class distance_type
  {
    cityblock,
    chessboard,
    quasieuclidean
  };

  dim_vector operator+(const dim_vector& dim, octave_idx_type val)
  {
    dim_vector result = dim;

    for (int i = 0; i < dim.length(); i++)
      result(i) += val;

    return result;
  }

  template <typename ResultType, typename  IndexType, typename ImageType >
  class curvdist2D
  {
  public:

    using element_type = std::pair<octave_idx_type, typename ResultType::element_type>;

    curvdist2D (const ImageType& image, int nargout, const boolNDArray & mask, const std::string& method = "chessboard")
    : f(), nargout (nargout)
    {
      dist_mat = ResultType(image.dims(), std::numeric_limits<typename ResultType::element_type>::infinity());

      if (image.numel () != 0)
        {
          if (nargout >= 2)
                idx_segment = IndexType (image.dims ());

          if (nargout == 3)
                idx_predecessor = IndexType (image.dims ());

          f = image;
          init_method (method);
          inheap = std::vector<char>  (image.numel (), (char)5);
          init_mask2D();
          initialize_from_seed (mask);
          do_curvdist();

        }

    }

    curvdist2D (const ImageType& image, int nargout, const Array<octave_idx_type> & C, const Array<octave_idx_type> & R, const std::string& method = "chessboard")
    : f(), nargout (nargout)
    {
      dist_mat = ResultType(image.dims(), std::numeric_limits<typename ResultType::element_type>::infinity());

      if (image.numel () != 0)
        {
          if (nargout >= 2)
                idx_segment = IndexType (image.dims ());

          if (nargout == 3)
                idx_predecessor = IndexType (image.dims ());

          f = image;
          init_method (method);
          inheap = std::vector<char>  (image.numel (), (char)5);
          init_mask2D();
          initialize_from_seed (C , R);
          do_curvdist();
        }
    }

    curvdist2D (const ImageType& image, int nargout, const Array<octave_idx_type> & ind, const std::string& method = "chessboard")
    : f(), nargout (nargout)
    {
      dist_mat = ResultType(image.dims(), std::numeric_limits<typename ResultType::element_type>::infinity());

      if (image.numel () != 0)
        {
          if (nargout >= 2)
                idx_segment = IndexType (image.dims ());

          if (nargout == 3)
                idx_predecessor = IndexType (image.dims ());

          f = image;
          init_method (method);
          inheap = std::vector<char>  (image.numel (), (char)5);
          init_mask2D();
          initialize_from_seed (ind);
          do_curvdist();
        }
    }

    const ResultType&
    value () const
    {
      return dist_mat;
    }

    octave_value_list
    get_result ()
    {
      return ovl (octave_value (value ()), octave_value (idx_segment), octave_value (idx_predecessor));
    }

  private:

    struct PointCmp
    {
      PointCmp ()  {}

      bool operator()(const element_type& a , const element_type& b ) const
      {return a.second > b.second;}
    };

    void init_method (const std::string& method)
    {
      if (method == "chessboard")
        {
          this->method = distance_type::chessboard;
        }
      else if (method == "cityblock")
        {
          this->method = distance_type::cityblock;
        }
      else if (method == "quasi-euclidean")
        {
          this->method = distance_type::quasieuclidean;
        }
      else
        {
          error ("curvdist: unregignized distance metric");
        }
    }

    void
    initialize_from_seed (const Array<octave_idx_type>& ind)
    {
      std::vector<element_type> cache;

      if (f.dim1 () == 1 || f.dim2 () == 1)
        {
          cache.reserve(ind.numel ()*2);
        }
      else
        cache.reserve((f.dim1 () + f.dim2 ()) * 2);

      Q = std::priority_queue<element_type, std::vector<element_type>, PointCmp>
      {PointCmp{}, std::move(cache)};

      try
        {
          for (octave_idx_type i = 0; i < ind.numel () ; i++)
            {
              dist_mat.checkelem(ind(i)-1) = 0;

              if (nargout >= 2)
                {
                  idx_segment.xelem(ind(i)-1) = ind(i);

                  if (nargout == 3)
                    idx_predecessor.xelem(ind(i)-1) = 0;
                }

              Q.push({ind(i)-1, 0});
            }
        }
      catch (...)
        {
          error ("out of range seed values");
        }
    }

    void
    initialize_from_seed (const Array<octave_idx_type>& C, const Array<octave_idx_type>& R)
    {
      if (C.numel () != R.numel ())
        error ("C and R should have equal sizes");

      std::vector<element_type> cache;

      if (f.dim1 () == 1 || f.dim2 () == 1)
        {
          cache.reserve(C.numel () * 2);
        }
      else
        cache.reserve((f.dim1 () + f.dim2 ()) * 2);

      Q = std::priority_queue<element_type, std::vector<element_type>, PointCmp>
      {PointCmp{}, std::move(cache)};

      const dim_vector& dim = f.dims();

      try
        {
          for (octave_idx_type i = 0; i < C.numel () ; i++)
            {
              octave_idx_type ind = ::compute_index (R.xelem(i)-1, C.xelem(i)-1 , dim);

              dist_mat(ind) = 0;

              if (nargout >= 2)
                {
                  idx_segment.xelem(ind) = ind + 1;

                  if (nargout == 3)
                    idx_predecessor.xelem(ind) = 0;
                }

              Q.push({ind, 0});
            }
        }
      catch (...)
        {
          error ("out of range seed values");
        }
    }

    void
    initialize_from_seed (const boolNDArray& mask)
    {
      if (mask.numel () != f.numel ())
        error ("mask and I should have equal sizes");


      std::vector<element_type> cache;

      if (f.dim1 () == 1 || f.dim2 () == 1)
        {
          octave_idx_type nseed = 0;

          for (octave_idx_type i = 0; i < mask.numel () ; i++)
            if (mask.xelem(i))
              ++nseed;

          cache.reserve(nseed * 2);
        }
      else
        cache.reserve((f.dim1 () + f.dim2 ()) * 2);

      Q = std::priority_queue<element_type, std::vector<element_type>, PointCmp>
      {PointCmp{}, std::move(cache)};

      for (octave_idx_type i = 0; i < mask.numel () ; i++)
        {
          if (mask.xelem(i))
            {
              dist_mat(i) = 0;

              if (nargout >= 2)
                {
                  idx_segment.xelem(i) = i + 1;

                  if (nargout == 3)
                    idx_predecessor.xelem(i) = 0;
                }

              Q.push({i, 0});
            }
        }
    }

    template<typename T>
    const std::vector<std::vector<T>>&
    create_quasi_euclidean_chamfer_weights() const
    {
      static const T h {1.0};
      static const T sq2 {2.0};
      static const std::vector<std::vector<T>> weight_quasi
        {
          {},
            {h,   sq2, h},
            {h,   h,   sq2, h,   sq2},
            {h,   sq2, h},
            {h,   sq2, h,   h,   sq2},
            {sq2, h,   sq2, h,   h,  sq2, h, sq2},
            {sq2, h,   h,   sq2, h},
            {h,   sq2, h},
            {sq2, h,   sq2, h,   h},
            {sq2, h,   h}

        };
      return weight_quasi;
    }

    std::vector<std::vector<octave_idx_type>>
    create_offset_to_neighbors(octave_idx_type dim1, bool only_direct_neghbors = false)
    {
      std::vector<std::vector<octave_idx_type>> offset_4
        {
          {},
          {1,dim1},
          {-1, 1, dim1},
          {-1,dim1},
          {-dim1, 1, dim1},
          {-dim1, -1 , 1, dim1},
          {-dim1,-1,dim1},
          {-dim1, 1},
          {-dim1,-1,1},
          {-dim1,-1}
        };

      std::vector<std::vector<octave_idx_type>> offset_8
        {
          {},
          {1,dim1,dim1+1},
          {-1,1,dim1 -1, dim1 , dim1 +1},
          {-1,dim1-1,dim1},
          {-dim1,-dim1+1, 1, dim1, dim1 +1},
          {-dim1 -1, -dim1, -dim1 +1, -1 , 1 , dim1-1, dim1 , dim1+1},
          {-dim1-1,-dim1,-1,dim1-1,dim1},
          {-dim1, -dim1 +1, 1},
          {-dim1-1,-dim1,-dim1+1,-1,1},
          {-dim1-1,-dim1,-1}
        };

      if (only_direct_neghbors)
        {
          return offset_4;
        }
      else
        {
          return offset_8;
        }
    }

    void
    init_mask2D()
    {
      const octave_idx_type dim1 = f.dim1();

      const octave_idx_type dim2 = f.dim2();

      if (dim1 == 1 || dim2 == 1)
        return;

      inheap[0] = 1;

      for (octave_idx_type i = 1; i < dim1 -1; i++)
        inheap[i] = 2;

      inheap[dim1-1] = 3;

      for (octave_idx_type i = 1; i < dim2-1 ; i++)
        inheap[i*dim1] = 4;

      for (octave_idx_type i = 1; i < dim2 -1; i++)
        inheap[i*dim1+dim1-1] = 6;

      inheap[dim1*dim2-dim1] = 7;

      for (octave_idx_type i = dim1*dim2-dim1+1; i < dim1*dim2 -1; i++)
        inheap[i] = 8;

      inheap[dim1*dim2 - 1] = 9;
    }

    void do_curvdist1D ()
    {
      octave_idx_type n = f.numel ();

      typename ResultType::element_type* dist = dist_mat.fortran_vec ();

      while (! Q.empty ())
        {
          auto u = Q.top ();

          Q.pop ();

          if (! inheap[u.first])
            {
              continue;
            }

          inheap[u.first] = 0;

          const bool quasi = method == distance_type::quasieuclidean;

          typename ResultType::element_type fu = f(u.first);

          for (auto p : {-1, 1})
            {
              auto v = u.first + p;

              if (v >= 0 && v < n && inheap[v])
                {
                  typename ResultType::element_type fv = f(v);

                  typename ResultType::element_type alt =
                    quasi ?
                    u.second + sqrt (pow(fu - fv,2) + 1) :
                    u.second + abs (fu - fv) + 1;

                  if (alt < dist[v])
                    {
                      dist[v] = alt;

                      if (nargout >= 2)
                        {
                          idx_segment.xelem(v) = idx_segment.xelem(u.first);

                          if (nargout == 3)
                            idx_predecessor.xelem(v) = u.first + 1;
                        }

                      Q.push({v, alt});
                    }
                }
            }
        }
    }

    void do_curvdist2D ()
    {
      const octave_idx_type dim1 = f.dim1();

      typename ResultType::element_type* dist = dist_mat.fortran_vec ();

      bool only_direct_neghbors = method == distance_type::cityblock;

      const auto& offset = create_offset_to_neighbors (dim1, only_direct_neghbors);

      if (method == distance_type::chessboard || method == distance_type::cityblock)
        {
          while (! Q.empty ())
            {
              auto u = Q.top ();

              Q.pop ();

              auto idx_to_w = inheap[u.first];

              if (! idx_to_w)
                {
                  continue;
                }

              inheap[u.first] = 0;

              const auto& neighbor_idx = offset[idx_to_w];

              typename ResultType::element_type fu = f(u.first);

              for (size_t i = 0 ; i < neighbor_idx.size(); i++)
                {
                  octave_idx_type v = u.first + neighbor_idx[i];

                  if (inheap[v])
                    {
                      typename ResultType::element_type fv = f(v);

                      typename ResultType::element_type alt = u.second + abs (fu - fv) + 1;

                      if (alt < dist[v])
                        {
                          dist[v] = alt;

                          if (nargout >= 2)
                            {
                              idx_segment.xelem(v) = idx_segment.xelem(u.first);

                              if (nargout == 3)
                                idx_predecessor.xelem(v) = u.first + 1;
                            }

                          Q.push({v, alt});
                        }
                    }
                }

              OCTAVE_QUIT;
            }
        }
      else if (method == distance_type::quasieuclidean)
        {
          const auto& weight_quasi = create_quasi_euclidean_chamfer_weights<typename ResultType::element_type> ();

          while (! Q.empty ())
            {
              auto u = Q.top ();

              Q.pop ();

              auto idx_to_w = inheap[u.first];

              if (! idx_to_w)
                {
                  continue;
                }

              inheap[u.first] = 0;

              const auto& neighbor_idx = offset[idx_to_w];

              const auto& wei = weight_quasi[idx_to_w];

              typename ResultType::element_type fu = f(u.first);

              for (size_t i = 0 ; i < neighbor_idx.size(); i++)
                {
                  auto v = u.first + neighbor_idx[i];

                  if (inheap[v])
                    {
                      typename ResultType::element_type fv = f(v);

                      typename ResultType::element_type alt = u.second + sqrt(wei[i] + pow(fu - fv, 2));

                      if (alt < dist[v])
                        {
                          dist[v] = alt;

                          if (nargout >= 2)
                            {
                              idx_segment.xelem(v) = idx_segment.xelem(u.first);

                              if (nargout == 3)
                                idx_predecessor.xelem(v) = u.first + 1;
                            }

                          Q.push({v, alt});
                        }
                    }
                }

              OCTAVE_QUIT;
            }
        }
    }

    void do_curvdist()
    {
      const octave_idx_type dim1 = f.dim1();

      const octave_idx_type dim2 = f.dim2();

      if (dim1 == 1 || dim2 == 1)
        {
           do_curvdist1D ();
        }
      else
        {
           do_curvdist2D ();
        }
    }

    ImageType f;

    const int nargout;

    ResultType dist_mat;

    IndexType idx_segment;

    IndexType idx_predecessor;

    std::priority_queue<std::pair<octave_idx_type, typename ResultType::element_type>,std::vector<std::pair<octave_idx_type, typename ResultType::element_type>>, PointCmp> Q;

    std::vector<char> inheap;

    distance_type method;
  };

  template <typename ResultType, typename  IndexType, typename ImageType>
  class curvdistND
  {
  public:
    struct queue_elem_type
    {
      octave_idx_type maskindex = 0;
      octave_idx_type imageindex = 0;
      typename ResultType::element_type imageval = 0;
    };

    struct PointCmpND
    {
      PointCmpND ()  {}

      bool operator()(const queue_elem_type& a , const queue_elem_type& b ) const
      {return a.imageval > b.imageval;}
    };

    curvdistND (const ImageType& image, int nargout, const boolNDArray & mask, const std::string& method = "chessboard")
    : f(), nargout (nargout)
    {
      dist_mat = ResultType(image.dims(), std::numeric_limits<typename ResultType::element_type>::infinity());

      if (image.numel () != 0)
        {
          if (nargout >= 2)
                idx_segment = IndexType (image.dims ());

          if (nargout == 3)
                idx_predecessor = IndexType (image.dims ());

          f = image;
          init_method (method);
          inheap  = create_zero_padded_maskND (image.dims ());
          initialize_from_seed (mask);
          do_curvdistND();
        }

    }
    curvdistND (const ImageType& image, int nargout, const Array<octave_idx_type> & C, const Array<octave_idx_type> & R, const std::string& method = "chessboard")
    : f(), nargout (nargout)
    {
      dist_mat = ResultType(image.dims(), std::numeric_limits<typename ResultType::element_type>::infinity());

      if (image.numel () != 0)
        {
          if (nargout >= 2)
                idx_segment = IndexType (image.dims ());

          if (nargout == 3)
                idx_predecessor = IndexType (image.dims ());

          f = image;
          init_method (method);
          inheap  = create_zero_padded_maskND (image.dims ());
          initialize_from_seed (C , R);
          do_curvdistND();
        }
    }

    curvdistND (const ImageType& image, int nargout, const Array<octave_idx_type> & ind, const std::string& method = "chessboard")
    : f(), nargout (nargout)
    {
      dist_mat = ResultType(image.dims(), std::numeric_limits<typename ResultType::element_type>::infinity());

      if (image.numel () != 0)
        {
          if (nargout >= 2)
                idx_segment = IndexType (image.dims ());

          if (nargout == 3)
                idx_predecessor = IndexType (image.dims ());

          f = image;
          init_method (method);
          inheap  = create_zero_padded_maskND (image.dims ());
          initialize_from_seed (ind);
          do_curvdistND();
        }
    }

    const ResultType&
    value () const
    {
      return dist_mat;
    }

    octave_value_list
    get_result ()
    {
      return ovl (octave_value (value ()), octave_value (idx_segment), octave_value (idx_predecessor));
    }

  private:

    octave_idx_type
    image_to_mask_index (octave_idx_type idximg, const std::vector<octave_idx_type>& cumimgdim, const std::vector<octave_idx_type>& cummskdim, octave_idx_type sumcummask)
    {
      octave_idx_type idxmsk = 0;

      for (octave_idx_type n = (octave_idx_type)(cumimgdim.size ())-2; n >= 0; n--)
        {
          idxmsk += idximg / cumimgdim[n] * cummskdim[n];

          idximg %= cumimgdim[n];
        }

      return idxmsk + idximg + sumcummask + 1  ;
    }

    std::tuple<std::vector <octave_idx_type>,std::vector <octave_idx_type>, octave_idx_type>
    create_cumulative_dims(const dim_vector& dimimg, const dim_vector& dimmsk)
    {
      std::vector <octave_idx_type> cumdimimg(dimimg.length());

      std::vector <octave_idx_type> cumdimmsk(dimimg.length());

      octave_idx_type sumcummask = 0;

      cumdimimg[0] = dimimg.xelem(0);

      cumdimmsk[0] = dimmsk.xelem(0);

      for (size_t i = 1 ; i < cumdimimg.size(); i++)
        {
            cumdimimg[i] = dimimg.xelem(i)*cumdimimg[i-1];

            cumdimmsk[i] = dimmsk.xelem(i)*cumdimmsk[i-1];

            sumcummask += cumdimmsk[i-1];
        }

      return {cumdimimg, cumdimmsk, sumcummask};
    }

    void
    init_method (const std::string& method)
    {
      if (method == "chessboard")
        {
          this->method = distance_type::chessboard;
        }
      else if (method == "cityblock")
        {
          this->method = distance_type::cityblock;
        }
      else if (method == "quasi-euclidean")
        {
          this->method = distance_type::quasieuclidean;
        }
      else
        {
          error ("curvdist: unregignized distance metric");
        }
    }

    void
    initialize_from_seed (const Array<octave_idx_type>& ind)
    {
      const dim_vector& dim = f.dims();

      std::vector<queue_elem_type> cache;

      cache.reserve(dim.numel() - (dim+(-2)).numel());

       Q =std::priority_queue<queue_elem_type,std::vector<queue_elem_type>, PointCmpND>
         {PointCmpND{}, std::move(cache)};

      auto cum = create_cumulative_dims(dim, dim+2);

      try
        {
          for (octave_idx_type i = 0; i < ind.numel () ; i++)
            {
              dist_mat.checkelem(ind(i)-1) = 0;

              if (nargout >= 2)
                {
                  idx_segment.xelem(ind(i)-1) = ind(i);

                  if (nargout == 3)
                    idx_predecessor.xelem(ind(i)-1) = 0;
                }

              Q.push({image_to_mask_index (ind(i)-1,std::get<0>(cum), std::get<1>(cum),std::get<2>(cum)), ind(i)-1, 0});
            }
        }
      catch (...)
        {
          error ("out of range seed values");
        }
    }

    void
    initialize_from_seed (const Array<octave_idx_type>& C, const Array<octave_idx_type>& R)
    {
      if (C.numel () != R.numel ())
        error ("C and R should have equal sizes");

      const dim_vector& dim = f.dims();

      std::vector<queue_elem_type> cache;

      cache.reserve(dim.numel() - (dim+(-2)).numel());

       Q = std::priority_queue<queue_elem_type,std::vector<queue_elem_type>, PointCmpND>
         {PointCmpND{}, std::move(cache)};

      auto cum = create_cumulative_dims(dim, dim+2);

      try
        {
          for (octave_idx_type i = 0; i < C.numel () ; i++)
            {
              octave_idx_type ind = ::compute_index (R.xelem(i)-1, C.xelem(i)-1 , dim);

              dist_mat(ind) = 0;

              if (nargout >= 2)
                {
                  idx_segment.xelem(ind) = ind + 1;

                  if (nargout == 3)
                    idx_predecessor.xelem(ind) = 0;
                }

              Q.push({image_to_mask_index (ind,std::get<0>(cum), std::get<1>(cum),std::get<2>(cum)), ind, 0});
            }
        }
      catch (...)
        {
          error ("out of range seed values");
        }
    }

    void
    initialize_from_seed (const boolNDArray& mask)
    {
      if (mask.numel () != f.numel ())
        error ("mask and I should have equal sizes");

      const dim_vector& dim = f.dims();

      std::vector<queue_elem_type> cache;

      cache.reserve(dim.numel() - (dim+(-2)).numel());

      Q = std::priority_queue<queue_elem_type,std::vector<queue_elem_type>, PointCmpND>
         {PointCmpND{}, std::move(cache)};

      auto cum = create_cumulative_dims(dim, dim+2);

      for (octave_idx_type i = 0; i < mask.numel () ; i++)
        {
          if (mask.xelem(i))
            {
              dist_mat(i) = 0;

              if (nargout >= 2)
                {
                  idx_segment.xelem(i) = i + 1;

                  if (nargout == 3)
                    idx_predecessor.xelem(i) = 0;
                }
              Q.push({image_to_mask_index (i,std::get<0>(cum), std::get<1>(cum),std::get<2>(cum)), i, 0});
            }
        }
    }

    template<typename T>
    std::vector<T>
    create_quasi_euclidean_chamfer_weights(const dim_vector& array_dims)
    {
      MArray<octave_idx_type> start(dim_vector(3,1));

      start(0) = 1;

      start(1) = 0;

      start(2) = 1;

      auto dist = start;

      for (int i = 2; i <= array_dims.length (); i++)
        {
          dim_vector shpsz = dim_vector::alloc(i);

          for (int j = 0; j < i-1; j++)
            shpsz(j) = 1;

          shpsz(i-1) = 3;

          dist = dist + start.reshape(shpsz);
        }

      const octave_idx_type result_size = pow(3,array_dims.length());

      std::vector<T> wei (result_size-1);

      octave_idx_type s = 0;

      for (int j = 0; j < result_size; j++)
        {
          if (j != result_size/2)
            wei[s++] = sqrt(dist(j))/2;
        }

      return wei;
    }

    std::vector<octave_idx_type>
    create_offset_to_neighbors(const dim_vector& dim, bool only_direct_neghbors = false)
    {
      octave_idx_type result_size;

      if (only_direct_neghbors)
        result_size = dim.length() * 2;
      else
        result_size = pow (3, dim.length ())-1;

      Array<octave_idx_type> idx (dim_vector (1, dim.length ()), 1);

      std::vector<octave_idx_type> offset (result_size);

      octave_idx_type center_idx = compute_index (idx, dim);

      if (only_direct_neghbors)
        {
          octave_idx_type k = 0;

          for (int i = 0; i < dim.length (); i++)
            {
              for (int j : {0, 2})
                {
                  idx(i) = j;

                  offset[k++] = compute_index (idx,dim) - center_idx;
                }

              idx(i) = 1;
            }
        }
      else
        {
          idx.fill(0);

          octave_idx_type k = 0;

          offset[k++] = -center_idx;

          for (int j = 1; j <= result_size; j++)
            {
              for (int i = 0; i < dim.length (); i++)
                {
                  if (++idx.xelem (i) == 3)
                    {
                      idx.xelem (i) = 0;
                    }
                  else
                    {
                      break;
                    }
                }

              if (j != (result_size+1)/2)
                offset[k++] = compute_index (idx,dim) - center_idx;
            }
        }

      return offset;
    }

    std::vector<bool>
    create_zero_padded_maskND( dim_vector dims)
    {
      // zero padded mask to handle boundary pixels
      octave_idx_type size = 1;

      for (int i = 0 ;i < dims.length (); i++)
        {
          dims(i) += 2;

          size *= dims(i);
        }

      std::vector<bool> result(size, true);

      std::vector<std::pair<octave_idx_type,octave_idx_type>> bounds;

      Array<octave_idx_type> indexes(dim_vector(1,dims.length()));

      bounds.reserve(dims.length());

      for (octave_idx_type i = 0 ;i < dims.length(); i++)
        {
          bounds.emplace_back (0, dims(i));
        }

      octave_idx_type sz = size;

      for (octave_idx_type k = 0 ;k < dims.length(); k++)
        {
          sz = sz/dims(k);
          for (octave_idx_type pad : {octave_idx_type (1), dims(k)})
            {
              bounds[k] = {pad-1, pad};

              for (octave_idx_type s = 0 ;s < dims.length(); s++)
                {
                  indexes.xelem(s)=bounds[s].first;
                }

              result[compute_index(indexes,dims)] = false;

              for (octave_idx_type j = 1 ; j < sz; j++)
                {
                  for (int i = 0; i < dims.length(); i++)
                    {
                      if (++indexes.xelem(i) == bounds[i].second)
                        {
                          indexes.xelem(i) = bounds[i].first;
                        }
                      else
                        {
                          break;
                        }
                    }

                  result[compute_index(indexes,dims)] = false;
                }
            }
          sz = sz*(dims(k)-2);

          bounds[k] = {1, dims(k)-1};
        }

      return result;
    }

    void
    do_curvdistND ()
    {
      typename ResultType::element_type* dist = dist_mat.fortran_vec ();

      bool only_direct_neghbors = method == distance_type::cityblock;

      auto mask_neighbor_idx = create_offset_to_neighbors (f.dims () + 2, only_direct_neghbors);

      auto image_neighbor_idx = create_offset_to_neighbors (f.dims (), only_direct_neghbors);

      if (method == distance_type::chessboard || method == distance_type::cityblock)
        {
          while (! Q.empty ())
            {
              auto u = Q.top ();

              Q.pop ();

              if (! inheap[u.maskindex])
                {
                  continue;
                }

              inheap[u.maskindex] = 0;

              typename ResultType::element_type fu = f(u.imageindex);

              for (size_t i = 0 ; i < mask_neighbor_idx.size(); i++)
                {
                  auto vmask = u.maskindex + mask_neighbor_idx[i];

                  if (inheap[vmask])
                    {
                      auto vimage = u.imageindex + image_neighbor_idx[i];

                      typename ResultType::element_type fv = f(vimage);

                      typename ResultType::element_type alt = u.imageval + abs(fu - fv) + 1;

                      if (alt < dist[vimage])
                        {
                          dist[vimage] = alt;

                          if (nargout >= 2)
                            {
                              idx_segment.xelem(vimage) = idx_segment.xelem(u.imageindex);

                              if (nargout == 3)
                                idx_predecessor.xelem(vimage) = u.imageindex+1;
                            }

                          Q.push({vmask, vimage, alt});
                        }
                    }
                }

              OCTAVE_QUIT;
            }
        }
      else if (method == distance_type::quasieuclidean)
        {
          auto wei = create_quasi_euclidean_chamfer_weights<typename ResultType::element_type> (f.dims ());

          while (! Q.empty ())
            {
              auto u = Q.top ();

              Q.pop ();

              if (! inheap[u.maskindex])
                {
                  continue;
                }

              inheap[u.maskindex] = 0;

              typename ResultType::element_type fu = f(u.imageindex);

              for (size_t i = 0 ; i < mask_neighbor_idx.size(); i++)
                {
                  auto vmask = u.maskindex + mask_neighbor_idx[i];

                  if (inheap[vmask])
                    {
                      auto vimage = u.imageindex + image_neighbor_idx[i];

                      typename ResultType::element_type fv = f(vimage);

                      typename ResultType::element_type alt = u.imageval + sqrt (wei[i] + pow(fu - fv, 2));

                      if (alt < dist[vimage])
                        {
                          dist[vimage] = alt;

                          if (nargout >= 2)
                            {
                              idx_segment.xelem(vimage) = idx_segment.xelem(u.imageindex);

                              if (nargout == 3)
                                idx_predecessor.xelem(vimage) = u.imageindex + 1;
                            }

                          Q.push({vmask, vimage, alt});
                        }
                    }
                }

              OCTAVE_QUIT;
            }
        }
    }

    ImageType f;

    const int nargout;

    ResultType dist_mat;

    IndexType idx_segment;

    IndexType idx_predecessor;

    std::priority_queue<queue_elem_type,std::vector<queue_elem_type>, PointCmpND> Q;

    std::vector<bool> inheap;

    distance_type method;
  };

  template <typename ResultType, typename IndexType, typename ImageType,  typename ... Args>
  octave_value_list do_curvdist (const ImageType& image, int nargout, Args...args)
  {
    const ImageType im = image.squeeze();

    octave_value_list retval;

    if (im.ndims () <= 2)
      retval = curvdist2D<ResultType, IndexType, ImageType>(im, nargout, args...).get_result ();
    else
      retval = curvdistND<ResultType, IndexType, ImageType>(im, nargout, args...).get_result ();

    retval(0) = retval(0).reshape(image.dims ());

    if (nargout >= 2)
      retval(1) = retval(1).reshape(image.dims ());

    if (nargout >= 3)
      retval(2) = retval(2).reshape(image.dims ());

    return retval;
  }

  template <typename ResultType, typename IndexType, typename ImageType,  typename ... Args>
  octave_value_list dispatch4 (const octave_value_list& unprocessed_args,int n,const ImageType& image, int nargout, Args...args)
  {
    octave_idx_type nargin = unprocessed_args.length ();

    if (nargin == 4)
      {
        if (unprocessed_args(n).is_string ())
          return do_curvdist<ResultType, IndexType> (image, nargout, args..., unprocessed_args(n).string_value ());
        else
          error ("invalid type for 'method'");
      }
    else
      return do_curvdist<ResultType, IndexType> (image, nargout, args...);
  }

  template <typename ResultType, typename IndexType, typename ImageType,  typename ... Args>
  octave_value_list dispatch3 (const octave_value_list& unprocessed_args,int n,const ImageType& image, int nargout, Args...args)
  {
    octave_idx_type nargin = unprocessed_args.length ();

    if (nargin >= 3 && n < 3)
      {
        if (unprocessed_args(n).is_string ())
          return do_curvdist< ResultType, IndexType> (image, nargout, args..., unprocessed_args(n).string_value ());
        else if (unprocessed_args(n).isnumeric ())
          return dispatch4< ResultType, IndexType> (unprocessed_args, n+1, image, nargout, args...,  unprocessed_args(n).octave_idx_type_vector_value ());
        else
          error ("invalid type for argument number %s", std::to_string (n+1).c_str ());
      }
    else
      return do_curvdist<ResultType, IndexType> (image, nargout, args...);
  }

  template < typename ResultType, typename IndexType, typename ImageType, typename ... Args>
  octave_value_list dispatch2 (const octave_value_list& unprocessed_args,int n,const ImageType& image, int nargout, Args...args)
  {
    octave_idx_type nargin = unprocessed_args.length ();

    if (nargin >= 2 && n < 2)
      {
        if (unprocessed_args(n).islogical ())
          return dispatch3< ResultType,IndexType> (unprocessed_args, n+1, image, nargout, args..., unprocessed_args(n).bool_array_value ());
        else if (unprocessed_args(n).isnumeric ())
          return dispatch3< ResultType,IndexType> (unprocessed_args, n+1,image, nargout, args...,  unprocessed_args(n).octave_idx_type_vector_value ());
        else
          error ("invalid type for argument number %s", std::to_string (n+1).c_str ());
      }

    return {};
  }

  template <typename IndexType>
  octave_value_list dispatch (const octave_value_list& args, int nargout)
  {
    octave_idx_type nargin = args.length ();

    if (nargin < 2 || nargin > 4)
      error ("invalid number of arguments");

    octave_value im = args(0);

    if (im.islogical ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.bool_array_value (), nargout);
    else if (im.is_int8_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.int8_array_value (), nargout);
    else if (im.is_int16_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.int16_array_value (), nargout);
    else if (im.is_int32_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.int32_array_value (), nargout);
    else if (im.is_int64_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.int64_array_value (), nargout);
    else if (im.is_uint8_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.uint8_array_value (), nargout);
    else if (im.is_uint16_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.uint16_array_value (), nargout);
    else if (im.is_uint32_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.uint32_array_value (), nargout);
    else if (im.is_uint64_type ())
      return image::dispatch2 < FloatNDArray,IndexType>(args, 1, im.uint64_array_value (), nargout);
    else if (im.isreal ())
      {
        if (im.is_single_type ())
          return image::dispatch2 < FloatNDArray,IndexType>(args, 1,im.float_array_value (), nargout);
        else
          return image::dispatch2 < NDArray,IndexType>(args, 1, im.array_value (), nargout);
      }
    else if (im.iscomplex ())
      {
        error ("invalid complex value input");
      }
    return octave_value_list ();
  }
}

DEFUN_DLD (curvdist, args, nargout,
           R"helpdoc(-*- texinfo -*-
@deftypefn {Loadable Function} {T =} curvdist(@var{I}, @var{mask})
@deftypefnx {Loadable Function} {T =} curvdist(@var{I}, @var{C}, @var{R})
@deftypefnx {Loadable Function} {T =} curvdist(@var{I}, @var{ind})
@deftypefnx {Loadable Function} {T =} curvdist(@var{___}, @var{method})
@deftypefnx {Loadable Function} {[T, idx] =} curvdist(@var{___})
@deftypefnx {Loadable Function} {[T, idx, pred] =} curvdist(@var{___})

Compute weighted distance transform on curved space for image.

The image @var{I} can be a multidimensional array of non negative values.

The seed points can be provided in three ways:

@table @asis
@item @qcode{@var{mask}}
A logical matrix with the same size as @var{I} that true values correspond to seed points
@item @qcode{@var{C}, @var{R}}
Indexes of columns and rows of the seed points
@item @qcode{@var{ind}}
Linear indexes
@end table

The return value @var{T} is a gray level image where the value of each point corresponds
 to its shortest distance to the provided seed points. The weighted distance on curved space corresponds to the length of the shortest geodesic path 'P' between points 'a' and 'b' [1].

The cost of each step differs based on the chosen distance metric @var{METHOD} that is one of "chessboard"(default)| "cityblock" | "quasi-euclidean".:@*

@table @asis
@item @qcode{"chessboard"}

@code{abs (I(p) - I(q)) + SpatialDistance(pq)}@*

@item @qcode{"cityblock"}

@code{abs (I(p) - I(q)) + SpatialDistance(pq)}@*

@item @qcode{"quasi-euclidean"}

@code{sqrt ((I(p) - I(q))^2 + SpatialDistance(pq)^2)}@*

where  I(x) is the gray value of the point x and SpatialDistance(pq) shows the spatial
distance between points p and its neighbor q.@*
@end table

SpatialDistance also differs based on the chosen distance metric.
In a 2D image the spatial distance between the center pixel p and its 8 neighbors is computed as:

@table @asis
@item @qcode{"chessboard"}

1 1 1@*
1 p 1@*
1 1 1@*

@item @qcode{"cityblock"}

inf 1@ @ inf@*
1@ @ @ p@ @ 1@*
inf 1@ @ inf@*

@item @qcode{"quasi-euclidean"}

sq2 1@ @ sq2@*
1@ @ @ p@ @ 1@*
sq2 1@ @ sq2@*

where sq2 is equal to sqrt(2).
@end table

@var{idx} is index of the nearest seed point. @*
@var{pred} is the predecessor map. pred(a) is the predecessor of 'a' in the shortest paths tree (that starts from seed points). In other words 'a' is the nearest neghbor to pred(a). Its value in seed points is zero.@*
The type of @var{T} is double if the type of @var{I} is double. For other input types the type of output is single.@*
The type of @var{idx} and @var{pred} depends on the size of the image. For an image of size less than 2^32 it is 'uint32' .For larger images it is 'uint64'.

[1] Fouard C., Gedda M. (2006) An Objective Comparison Between Gray Weighted Distance Transforms and Weighted Distance Transforms on Curved Spaces. In: Kuba A., Nyúl L.G., Palágyi K. (eds) Discrete Geometry for Computer Imagery. DGCI 2006. Lecture Notes in Computer Science, vol 4245. Springer, Berlin, Heidelberg.

@seealso{bwdist, graydist}
@end deftypefn)helpdoc")
{


  octave_idx_type nargin = args.length ();

  if (nargin < 2 || nargin > 4)
    error ("invalid number of arguments");

  octave_value im = args(0);

  if (static_cast<unsigned long long> (im.numel ()) <= 0xFFFFFFFF)
    return image::dispatch<uint32NDArray>(args, nargout);
  else
    return image::dispatch<uint64NDArray>(args, nargout);
}