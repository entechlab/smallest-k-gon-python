#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>

#include "include/convex.h"

namespace py = pybind11;
py::array_t<float> py_smallest_kgon(
    py::array_t<float, py::array::c_style | py::array::forcecast> input_points,
    unsigned int k) {
  // Request buffer info from the input NumPy array
  py::buffer_info buf_info = input_points.request();

  // Check dimensions (must be N x 2)
  if (buf_info.ndim != 2 || buf_info.shape[1] != 2) {
    throw std::runtime_error("Input array must be of shape (N, 2)");
  }

  int n = buf_info.shape[0];
  if (k <= 0) {
    throw std::runtime_error("h (expected hull vertices) must be positive");
  }
  // A reasonable upper bound, though Chan's works even if h is larger than
  // actual if (h_expected > n) {
  //     throw std::runtime_error("h cannot be greater than the number of points
  //     n");
  // }

  // Get pointer to the data
  float *ptr = static_cast<float *>(buf_info.ptr);

  // Copy data into std::vector<Point>
  std::vector<ayw::float2> points(n);
  for (int i = 0; i < n; ++i) {
    points[i] = {ptr[i * 2], ptr[i * 2 + 1]};
  }

  // Call the c++ smallest-k-gon
  ayw::convex n_gon;
  n_gon.build(points.begin(), points.end());

  ayw::convex kgon = n_gon;
  kgon.simplify(k);
  // Convert result back to NumPy array
  int num_vertices = kgon.vertices.size();
  py::array_t<float> result = py::array_t<float>({num_vertices, 2});
  py::buffer_info res_buf_info = result.request();
  float *res_ptr = static_cast<float *>(res_buf_info.ptr);

  for (int i = 0; i < num_vertices; ++i) {
    res_ptr[i * 2] = kgon.vertices[i].x;
    res_ptr[i * 2 + 1] = kgon.vertices[i].y;
  }

  return result;
}

// Define the Python module
PYBIND11_MODULE(smallest_kgon, m) {
  m.doc() = "Python module wrapping C++ Smallest Kgon enclosing N-gon";
  m.def("smallest_kgon", &py_smallest_kgon,
        "Computes the smallest kgon circumbscribing a N-gon using Agrawal's "
        "algorithm.\n\n"
        "Args:\n"
        "    points (numpy.ndarray): An N x 2 NumPy array of points.\n"
        "    h (int): The expected number of vertices in Kgon (k).\n\n"
        "Returns:\n"
        "    numpy.ndarray: A H x 2 NumPy array containing the Kgon "
        "vertices (k <=N).",
        py::arg("points"), py::arg("k"));
}
