#ifndef PDFH
#define PDFH

#include "onbh.h"
#include "hitable.h"
#include "rng.h"

class pdf {
public: 
  virtual float value(const vec3& direction) const = 0;
  virtual vec3 generate(random_gen& rng) = 0;
};

class cosine_pdf : public pdf {
public:
  cosine_pdf(const vec3& w) {
    uvw.build_from_w(w);
  }
  virtual float value(const vec3& direction) const {
    float cosine = dot(unit_vector(direction), uvw.w());
    if(cosine > 0) {
      return(cosine/M_PI);
    } else {
      return(0);
    }
  } 
  virtual vec3 generate(random_gen& rng) {
    return(uvw.local(rng.random_cosine_direction()));
  }
  onb uvw;
};

class hitable_pdf : public pdf {
public:
  hitable_pdf(hitable *p, const vec3& origin) : ptr(p), o(origin) {}
  virtual float value(const vec3& direction) const {
    return(ptr->pdf_value(o,direction));
  }
  virtual vec3 generate(random_gen& rng) {
    return(ptr->random(o)); 
  }
  hitable *ptr;
  vec3 o;
};

class mixture_pdf : public pdf {
public:
  mixture_pdf(pdf *p0, pdf *p1) {
    p[0] = p0;
    p[1] = p1;
  }
  virtual float value(const vec3& direction) const {
    return(0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction));
  }
  virtual vec3 generate(random_gen& rng) {
    if(rng.unif_rand() < 0.5) {
      return(p[0]->generate(rng));
    } else {
      return(p[1]->generate(rng));
    } 
  }
  pdf *p[2];
};

#endif