#ifndef LAB1231_SUN_PRJ_SHOTTON_LOSS_FUNCTOR_H_
#define LAB1231_SUN_PRJ_SHOTTON_LOSS_FUNCTOR_H_

namespace lab1231_sun_prj {
namespace shotton {

/*
The class ZeroOneLossFunctor is used as a functor to be attached to the OPENGM's factor graph
whenever a loss augmented inference is required.

In loss augmented inference, we aim to maximize the loss.
It is equivalent to minimize the negative loss.
Thus:
y_star = argmin E(.) + (-loss)

Due to the limitation of OPENGM implementation: cannot accomodate a factor connecting all pixels (the highest term),
the operator() always returns the minimum loss, i.e. -1.0, for the last node whenever active=true.
It is the case for all the time, except for one occasion when y[i]==ytrue[i] for all element i.

Due to always returning (loss = -1.0), we get y_star _wrong_ at occasions when the inference algorithm _ever_
(with low probability) _evaluates_ y==ytrue _and_ $E(y==ytrue) + (-1.0) > E(y') + (-1.0), \forall y' \in Y\y$.
*/
template<typename T>
class ZeroOneLossFunctor {
 public:
   ZeroOneLossFunctor(bool last_node, bool active)
      : last_node_(last_node), active_(active) { }
   ~ZeroOneLossFunctor() {}

   template<typename Iterator>
   inline const T operator()(Iterator begin) const {
      if (last_node_ and active_) return -1.0;
      else return 0.0;
   }

 private:
   bool last_node_;
   bool active_;
};

}
}

#endif
