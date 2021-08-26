testImage: missc
	gcc -c iris_image_compression_test.c -I ../include
	mv *.o ../out
	cd ../out && gcc *.o -o ImageCompressiontesting.out
	cd ../out && ./ImageCompressiontesting.out

missc:
		cd ../source && gcc -c iris_buffer.c iris_compression.c iris_heuristic.c  iris_utils.c iris_api.c iris_cloud_detection.c iris_entropy_encoder.c iris_predictor.c -I ../include -w
		mv ../source/*.o ../out/

cleanDir:
	rm -f  ../out/*.out ../out/*.o ../out/*.fl
	
	
	