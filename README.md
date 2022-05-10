# Rate Calculator for Muon base triggers

This is a simple calulator for muon triggers to estimate firing rate.

## Function definition


```
HltInput.h
```
HltInput class will fetch 'hltanalysis/HltTree' and trigger object in 'hltobject/$(TRIGGERNAME)'.

```
RateCalc.h
```
RateCalc class inherits HltInput, getters and calculation are all included in here
