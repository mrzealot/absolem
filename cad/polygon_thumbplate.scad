

difference() {
	difference() {
		offset(chamfer = false, delta = -10) {
			offset(chamfer = false, delta = 13) {
				translate(v = [10.2500000000, 0, 0]) {
					translate(v = [9.2500000000, 0, 0]) {
						translate(v = [0, 35, 0]) {
							union() {
								translate(v = [-9.2500000000, 0, 0]) {
									square(size = [18.5000000000, 14]);
								}
								translate(v = [0, -65, 0]) {
									rotate(a = -23.7213372369) {
										translate(v = [0, 65, 0]) {
											translate(v = [-9.2500000000, 0, 0]) {
												square(size = [18.5000000000, 14]);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		translate(v = [2.2500000000, 0, 0]) {
			translate(v = [10.2500000000, 0, 0]) {
				translate(v = [7.0000000000, 0, 0]) {
					translate(v = [0, 35, 0]) {
						union() {
							translate(v = [-7.0000000000, 0, 0]) {
								square(size = [14, 14]);
							}
							translate(v = [0, -65, 0]) {
								rotate(a = -23.7213372369) {
									translate(v = [0, 65, 0]) {
										translate(v = [-7.0000000000, 0, 0]) {
											square(size = [14, 14]);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	translate(v = [10.2500000000, 0, 0]) {
		translate(v = [9.2500000000, 0, 0]) {
			translate(v = [0, -30, 0]) {
				rotate(a = -11.8606686184) {
					translate(v = [0, 74.1000000000, 0]) {
						circle($fn = 1000, d = 2.5000000000);
					}
				}
			}
		}
	}
}