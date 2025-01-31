#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;
#include "new.cpp"

// using namespace std;

template <typename T> py::array_t<T> vector_to_numpy(std::vector<T> &&vec) {
  py::array_t<T> result({vec.size()}, {sizeof(T)});
  auto result_buffer = result.request();
  T *result_ptr = static_cast<T *>(result_buffer.ptr);
  std::move(vec.begin(), vec.end(), result_ptr);
  return result;
}

template <typename T>
py::array_t<T> vector_to_numpy(std::vector<T> &&vec, size_t rows, size_t cols) {
  py::array_t<T> result({rows, cols});
  auto result_buffer = result.request();
  T *result_ptr = static_cast<T *>(result_buffer.ptr);
  std::move(vec.begin(), vec.end(), result_ptr);

  return result;
}

py::tuple run(int L, int T, int N, int kstep, int kon, int koff) {
  Simulation sim(L, T, N, kstep, kon, koff);
  sim.run();
  return py::make_tuple(vector_to_numpy(std::move(sim.OUTPUT), 10, N));
}

PYBIND11_MODULE(mysim, m) { m.def("run", &run, "Runs a simulation"); }