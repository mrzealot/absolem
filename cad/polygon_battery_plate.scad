

union() {
	translate(v = [0, 39.0000000000, 0]) {
		translate(v = [52.5000000000, 0, 0]) {
			square(size = [36, 30]);
		}
	}
	difference() {
		translate(v = [0, 46.1000000000, 0]) {
			translate(v = [45, 0, 0]) {
				square(size = [51, 22.9000000000]);
			}
		}
		translate(v = [0, 46.1000000000, 0]) {
			translate(v = [45, 0, 0]) {
				union() {
					union() {
						union() {
							translate(v = [2.5000000000, 0, 0]) {
								translate(v = [0, 2.5000000000, 0]) {
									circle($fn = 1000, d = 2.5000000000);
								}
							}
							translate(v = [2.5000000000, 0, 0]) {
								translate(v = [0, 20.4000000000, 0]) {
									circle($fn = 1000, d = 2.5000000000);
								}
							}
						}
						translate(v = [48.5000000000, 0, 0]) {
							translate(v = [0, 2.5000000000, 0]) {
								circle($fn = 1000, d = 2.5000000000);
							}
						}
					}
					translate(v = [48.5000000000, 0, 0]) {
						translate(v = [0, 20.4000000000, 0]) {
							circle($fn = 1000, d = 2.5000000000);
						}
					}
				}
			}
		}
	}
}