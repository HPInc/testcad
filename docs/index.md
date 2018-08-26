---
layout: default
---

Testing is **learning** about a system up to a point where there is enough information about it as to provide an objective criteria upon which to make a decission: _Go or stop_.

An objective criteria requires **quantitative** data and must have solid grounds to minimize the **risk** of failure because such risk is [always](https://www.testingexcellence.com/seven-principles-of-software-testing/) present.

### Control your system

Mastering any system requires **time** and usually we don't have that time. Hard learned [lessons](http://www.testingeducation.org/BBST/testdesign/KanerBachPettichord_Lessons_Learned_in_SW_testingCh3-1.pdf) of the past left us with a set of test **design techniques** to get as much information about a system as possible in the shorter time.

### Be effective

Applying test design techniques will not only make us more effective at testing but will also help us _learn from our mistakes_.

There are many test design techniques and tools for applying each of them out there, with testCAD we want to make as much techniques and tools possible closer to the test designer and make their use straight forward and intuitive. 

#### Techniques

*   Orthogonal arrays.
*   State-Transition N-1 switch.
*   Traceability matrixes.

* * *

#### testCAD UI concept

- Test group
  - Collection (Just a subset of variables you will use in a combination design)
  - Combination (Import collections or pick from tree entire branches to combine in several ways)
  - Sequence (Design your test as a workflow and generate N-1 switch test cases from the diagram)
  - Cross check (Create a matrix to trace between axes)
- Component (Part of your system)
  - Module (Menu entry)
    - Module (Sub Menu entry)
      - Element (Menu choice)
      - Element (Menu choice)
      - Element (Menu choice)
    - Module (Sub Menu entry)
      - Element (Menu choice)
      - Element (Menu choice)
      - Element (Menu choice)

#### Collaborate !

testCAD has been developed with [Open Source Qt](https://www.qt.io/download) you are invited to collaborate with our project we believe your work will make us improve.

testCAD is distributed under the [MIT License](https://choosealicense.com/licenses/mit/). 

![Branching](https://guides.github.com/activities/hello-world/branching.png)

