from svg_gcode import *
file="test/test_vector.svg"
file="/home/harvie/Work/Designs/cnc/loga/mardera.svg"
file="/home/harvie/Work/Designs/cnc/loga/logo_spoje.svg"
comp_instance = SVGcode(file)
comp_instance.find_paths()                        # Extract path coordinates
comp_instance.linearize_paths(curve_fidelity = 10)     # Convert paths into polygons
#comp_instance.write(outputfile = "hrv.svg")
#print(comp_instance.linear_coords[0])
for path in comp_instance.linear_coords:
	p = path[0]
	print("g0 x%s y%s"%(str(p[0]),str(p[1])))
	for p in path:
		print("g1 x%s y%s"%(str(p[0]),str(p[1])))
	print "( ---------- cut-here ---------- )"
