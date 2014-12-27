#ifndef LAB1231_SUN_PRJ_SHOTTON_LOSS_FUNCTOR_H_
#define LAB1231_SUN_PRJ_SHOTTON_LOSS_FUNCTOR_H_

namespace lab1231_sun_prj {
namespace shotton {

/*
The class ZeroOneLossFunctor is used as a functor to be attached to the OPENGM'sfactor graph
whenever a loss augmented inference is required.

In loss augmented inference, we aim to maximize the loss.
It is equivalent to minimize the negative loss.
Thus:
y_star = argmin E(.) + (-loss)

Notice that due to OPENGM implementation limitation (cannot accomodate the highest term),
the operator() always returns the minimum loss, i.e. -1.0 for the last node and whenever active=true.
This is a kind of approximations, but we will get it right almost all the time since loss=0 occurs only at once,
iff y_true[i] == y[i] for all element i.
That said, we will get it wrong at one occasion with low probability: 
when y*==y_true and (E(y*) + 0.0) < (E(y*) + (-1.0))
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
