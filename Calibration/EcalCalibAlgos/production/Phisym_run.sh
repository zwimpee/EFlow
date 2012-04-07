#!/bin/bash
mv AlCaPhiSym_Run2011$1-v1_RAW_$2_$3 AlCaPhiSym_Run2011$1-v1_RAW_$2_$3_DA
cp AlCaPhiSym_Run2011$1-v1_RAW_$2_$3_DA/jsonls-alcaphisym.txt .
./runphisymmetry2.sh caf /AlCaPhiSym/Run2011$1-v1/RAW Collisions11 $2 $3 GR_R_42_V21B::All
