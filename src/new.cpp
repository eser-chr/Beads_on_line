#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class Simulation {
private:
  using pos_t = uint16_t;
  using time_t = uint16_t;
  using kinId_t = uint16_t;
  using rNum_t = uint32_t;

  enum State : bool { UNBOUND, BOUND };
  enum Side : bool { NO_KIN, YES_KIN };
  struct Kinesin {
    pos_t position;
    State state;
  };

  rNum_t kon;
  rNum_t koff;
  rNum_t kstep;

  pos_t L;
  time_t T;
  time_t log_period;
  kinId_t N_kinesins;

  std::vector<Kinesin> kinesins;
  std::vector<Side> state_grid;
  std::vector<kinId_t> order;

  std::mt19937 rng;
  const size_t divider = 1000000;
  std::uniform_int_distribution<rNum_t> distribution;


  // BUFFERS

  pos_t _pos;
  kinId_t _kin_id;
  State _state;
  rNum_t _r;

public:
  std::vector<pos_t> OUTPUT;

private:
  inline void addKinesin(kinId_t kin_id, pos_t pos, State state) {
    kinesins[kin_id].position = pos;
    kinesins[kin_id].state = state;
    state_grid[pos] = Side::YES_KIN;
  }

  inline void bind() {
    state_grid[_pos] = Side::YES_KIN;
    kinesins[_kin_id].state = BOUND;
  }

  inline void unbind() {
    state_grid[_pos] = Side::NO_KIN;
    kinesins[_kin_id].state = State::UNBOUND;
  }

  inline void step() {
    state_grid[_pos] = Side::NO_KIN;
    state_grid[_pos + 1] = Side::YES_KIN;
    kinesins[_kin_id].position = _pos + 1;
  }

  inline void tryToStep() {
    if (_state == State::BOUND && state_grid[_pos + 1] == Side::NO_KIN &&
        _pos + 1 < L) {
      _r = distribution(rng);
      if (_r < kstep)
        step();
    }
  }

  void kinesinAction() {
    if (_state == State::UNBOUND) {
      _r = distribution(rng);
      if (_r < kon)
        bind();
    } else {
      _r = distribution(rng);
      if (_r < koff)
        unbind();
    }
    tryToStep();
  }

  void writing(time_t iter) {
    if (iter % log_period == 0) {
      int idx = iter / log_period;
      idx *= N_kinesins;
      for (kinId_t kin = 0; kin < N_kinesins; ++kin) {
        OUTPUT[idx + kin] = kinesins[kin].position;
      }
    }
  }

public:
  void run() {
    for (time_t iter = 0; iter < T; ++iter) {
      writing(iter); // Write pos of every kinesin
      std::shuffle(order.begin(), order.end(), rng);

      for (kinId_t kin : order) {
        _kin_id = kin;
        _pos = kinesins[_kin_id].position;
        _state = kinesins[_kin_id].state;
        kinesinAction();
      }
    }
  }
  Simulation(pos_t L, time_t T, kinId_t N_kinesins, rNum_t kstep, rNum_t kon,
             rNum_t koff)
      : L(L), T(T), N_kinesins(N_kinesins), kon(kon), koff(koff), kstep(kstep),
        kinesins(std::vector<Kinesin>(N_kinesins)),
        state_grid(std::vector<Side>(L)), rng(std::random_device{}()),
        log_period(T / 10), distribution(0, divider) {
    for (size_t i = 0; i < N_kinesins; ++i)
      addKinesin(i, i + 3000, State::BOUND);

    order.resize(N_kinesins);
    std::iota(order.begin(), order.end(), 0);

    OUTPUT.resize(N_kinesins * 10);
  }
};