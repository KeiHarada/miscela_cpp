miscela: miscela.o Sensor.o Correlation.o input.o extracting.o segmenting.o picking.o findAssociation.o findAssociation_base.o Cluster.o clustering.o threshold.o distance.o evaluation.o
	g++ -Wl,--stack,4194304 -Wall -o miscela miscela.o Sensor.o Correlation.o input.o extracting.o segmenting.o picking.o findAssociation.o findAssociation_base.o Cluster.o clustering.o threshold.o distance.o evaluation.o
miscela.o: miscela.cpp
	g++ -Wall -c miscela.cpp -o miscela.o
Sensor.o: Sensor.cpp
	g++ -Wall -c Sensor.cpp -o Sensor.o
Correlation.o: Correlation.cpp
	g++ -Wall -c Correlation.cpp -o Correlation.o
Cluster.o: Cluster.cpp
	g++ -Wall -c Cluster.cpp -o Cluster.o
clustering.o: clustering.cpp
	g++ -Wl,--stack,4194304 -Wall -c clustering.cpp -o clustering.o $(OPTICS) $(BOOST) $(FP) $(GEOMETRY) $(THREADS)
threshold.o: threshold.cpp
	g++ -Wall -c threshold.cpp -o threshold.o
distance.o: distance.cpp
	g++ -Wall -c distance.cpp -o distance.o
input.o: input.cpp
	g++ -Wall -c input.cpp -o input.o
extracting.o: extracting.cpp
	g++ -Wall -c extracting.cpp -o extracting.o
segmenting.o: segmenting.cpp
	g++ -Wall -c segmenting.cpp -o segmenting.o
picking.o: picking.cpp
	g++ -Wall -c picking.cpp -o picking.o
findAssociation.o: findAssociation.cpp
	g++ -Wall -c findAssociation.cpp -o findAssociation.o
findAssociation_base.o: findAssociation_base.cpp
	g++ -Wall -c findAssociation_base.cpp -o findAssociation_base.o
evaluation.o: evaluation.cpp
	g++ -Wall -c evaluation.cpp -o evaluation.o
buildtype := release
ifeq ($(buildtype),release)
	CXXFLAGS += -O3
else ifeq ($(buildtype),debug)
	CXXFLAGS += -O0 -g
else
 	$(error buildtype must be release, debug, profile or coverage)
endif
OUTDIR := Build/$(buildtype)

OPTICS = -I ./
THREADS = -I "C:/Users/OnizukaLab/cpp_include/MingwThreads"
FP = -I "C:/Users/OnizukaLab/cpp_include/FunctionalPlus/include"
GEOMETRY = -I "C:/Users/OnizukaLab/cpp_include/Geometry/include"
BOOST = -I "C:/Users/OnizukaLab/boost_1_66_0/boost/stage/include/boost-1_66"
clean:
	-del *.o
