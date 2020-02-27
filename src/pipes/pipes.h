#ifndef PIPES_H
#define PIPES_H

#include <memory>
#include <functional>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <array>
#include <algorithm>
#include <optional>

namespace pipes {
  template <typename T>
  using VectorPtr = std::shared_ptr<std::vector<T>>;
  template <typename T, size_t N>
  using ArrayPtr = std::shared_ptr<std::array<T,N>>;

  template <typename S>
  class Pipe {
  protected:
    VectorPtr<S> source;
  public:
    Pipe(std::vector<S>& s) : source {std::make_shared<std::vector<S>>(s)} {}
    Pipe(std::vector<S>&& s) : source {std::make_shared<std::vector<S>>(s)} {}
    Pipe(std::set<S>& s) : Pipe {std::vector<S>(s.begin(), s.end())} {}
    Pipe(std::set<S>&& s) : Pipe {std::vector<S>(s.begin(), s.end())} {}
    template <size_t N>
    Pipe(std::array<S,N>& s) : Pipe {std::vector<S>(s.begin(), s.end())} {}
    template <size_t N>
    Pipe(std::array<S,N>&& s) : Pipe {std::vector<S>(s.begin(), s.end())} {}
    Pipe(VectorPtr<S> s) : source {s} {}
    template <typename F>
    void forEach(F f) {
      for (S& s : *source) {
        f(s);
      }
    }
    template <typename D, typename F>
    Pipe<D> map(F mapper) {
      std::vector<D> result {};
      result.reserve(source->size());
      std::transform(source->begin(), source->end(), std::back_inserter(result), mapper);
      return Pipe<D> {std::make_shared<std::vector<D>>(result)};
    }
    template <typename D, typename F>
    Pipe<D> flatMap(F mapper) {
      std::vector<D> result {};
      for (S& s : *source) {
        std::vector<D> partial {mapper(s)};
        std::copy(partial.begin(), partial.end(), std::back_inserter(result));
      }
      return Pipe<D> {std::make_shared<std::vector<D>>(result)};
    }
    template <typename F>
    Pipe<S> filter(F filter) {
      std::vector<S> result {};
      result.reserve(source->size());
      std::copy_if(source->begin(), source->end(), std::back_inserter(result), filter);
      return Pipe<S> {std::make_shared<std::vector<S>>(result)};
    }
    Pipe<S> take(int n) {
      std::vector<S> result {};
      int toKeep = source->size();
      toKeep = std::max(0, std::min(n, toKeep));
      result.reserve(toKeep);
      std::copy_n(source->begin(), toKeep, std::back_inserter(result));
      return Pipe<S> {std::make_shared<std::vector<S>>(result)};
    }
    template <typename P>
    Pipe<S> takeWhile(P predicate) {
      std::vector<S> result {};
      result.reserve(source->size());
      for (S& s : *source) {
        if (!predicate(s)) break;
        result.push_back(s);
      }
      return Pipe<S> {std::make_shared<std::vector<S>>(result)};
    }
    Pipe<S> drop(int n) {
      std::vector<S> result {};
      int toKeep = source->size();
      toKeep -= std::max(0, std::min(n, toKeep));
      result.reserve(toKeep);
      std::copy_n(source->end()-toKeep, toKeep, std::back_inserter(result));
      return Pipe<S> {std::make_shared<std::vector<S>>(result)};
    }
    template <typename P>
    Pipe<S> dropWhile(P predicate) {
      std::vector<S> result {};
      result.reserve(source->size());
      bool taking {false};
      for (S& s : *source) {
        if (!taking && predicate(s)) continue;
        taking = true;
        result.push_back(s);
      }
      return Pipe<S> {std::make_shared<std::vector<S>>(result)};
    }
    Pipe<S> reverse() {
      std::vector<S> result {};
      result.reserve(source->size());
      std::copy(source->rbegin(), source->rend(), std::back_inserter(result));
      return Pipe<S> {std::make_shared<std::vector<S>>(result)};
    }
    template <typename D, typename F>
    D collect(D z, F update) {
      D acc {z};
      for (S& s : *source) {
        acc = update(acc, s);
      }
      return acc;
    }
    template <typename P>
    std::optional<S> find(P predicate) {
      for (S& s : *source) {
        if (predicate(s)) return s;
      }
      return std::nullopt;
    }
    template <typename P>
    bool exists(P predicate) {
      for (S& s : *source) {
        if (predicate(s)) return true;
      }
      return false;
    }
    template <typename P>
    bool forAll(P predicate) {
      for (S& s : *source) {
        if (!predicate(s)) return false;
      }
      return true;
    }
    template <typename K, typename F>
    std::map<K,std::vector<S>> groupBy(F groupKey) {
      std::map<K,std::vector<S>> result {};
      for (S& s : *source) {
        result[groupKey(s)].push_back(s);
      }
      return result;
    }
    std::optional<S> max() {
      if (!source->size()) return std::nullopt;
      S& maxValue = (*source)[0];
      for (S& s : *source) {
        if (s > maxValue) maxValue = s;
      }
      return maxValue;
    }
    std::optional<S> min() {
      if (!source->size()) return std::nullopt;
      S& minValue = (*source)[0];
      for (S& s : *source) {
        if (s < minValue) minValue = s;
      }
      return minValue;
    }
    std::string join(std::string sep = "") {
      std::stringstream result {};
      bool first {true};
      for (S& s : *source) {
        if (!first) result << sep;
        first = false;
        result << s;
      }
      return result.str();
    }
    template <typename P>
    std::pair<std::vector<S>,std::vector<S>> partition(P predicate) {
      std::vector<S> satisfies {};
      std::vector<S> notSatisfies {};
      for (S& s : *source) {
        if (predicate(s)) {
          satisfies.push_back(s);
        } else {
          notSatisfies.push_back(s);
        }
      }
      std::pair<std::vector<S>,std::vector<S>> result {satisfies, notSatisfies};
      return result;
    }
    size_t size() { return source->size(); }
    bool isEmpty() { return source->empty(); }
    std::vector<S> toVector() const { return *source; }
    template <size_t N>
    std::array<S,N> toArray() const {
      std::array<S,N> result {};
      std::copy(source->begin(), source->end(), result.begin());
      return result;
    }
    std::set<S> toSet() const {
      std::set<S> result {};
      std::copy(source->begin(), source->end(), std::inserter(result, result.end()));
      return result;
    }
  };
}

#endif
