import numpy
import svg.path
import lxml.etree as ET

'''
SVG path linearization library (eg. for SVG2gcode use)
	Made by Tomas 'Harvie' Mudrunka 2018

heavily based on SVG compression library
	released under GPLv3?
	by Gen Del Raye (July 22, 2014): https://pypi.org/project/SVGCompress/
'''

class SVGcode:

	def __init__(self, svg_file):
		'''
		Read svg from path 'svg_file' (e.g. 'test/test_vector.svg')
		'''
		#self.filename, self.extension = svg_file.split('.') # Get filename and extension
		#assert self.extension == 'svg', 'File must be an svg'
		try:
			self.figure_data = ET.parse(svg_file) # Parse svg data as xml
		except ET.XMLSyntaxError, e:
			'''
			Large svgs may trigger lxml's 'excessive depth in document' exception
			'''
			warnings.warn('lxml error: %s - Trying huge xml parser' %(e.message))
			huge_parser = ET.XMLParser(huge_tree = True)
			self.figure_data = ET.parse(svg_file, parser = huge_parser)
		self.root = self.figure_data.getroot() # Root object in svg

	def find_paths(self):
		'''
		Find and parse nodes in the xml that correspond to paths in the svg
		'''
		tag_prefix = '{*}'
		self.path_nodes = self.root.findall('.//%spath' %(tag_prefix))
		self.paths = list()
		self.paths = [svg.path.parse_path(p.attrib.get('d', 'M 0,0 z')) for p in self.path_nodes]

	def linearize_paths(self, curve_fidelity = 10):
		'''
		Turn svg paths into discrete lines
		Inputs:
			curve_fidelity(int) - number of lines with which to approximate curves
				in svg. Higher values necessitates longer computation time.
		'''
		self.linear_coords = [self.linearize(p, curve_fidelity) for p in self.paths]


	def linearize_line(self, segment, n_interpolate = None):
		'''
		Turn svg line into set of coordinates by returning
		start and end coordinates of the line segment.
		n_interpolate is only used for consistency of use
		with linearize_curve()
		'''
		return numpy.array([segment.start, segment.end])

	def linearize_curve(self, segment, n_interpolate = 10):
		'''
		Estimate svg curve (e.g. Bezier, Arc, etc.) using
		a set of n discrete lines. n_interpolate sets the
		number of discrete lines per curve.
		'''
		interpolation_pts = numpy.linspace(0, 1, n_interpolate, endpoint = False)[1:]
		interpolated = numpy.zeros(n_interpolate + 1, dtype = complex)
		interpolated[0] = segment.start
		interpolated[-1] = segment.end
		for i, pt in enumerate(interpolation_pts):
				interpolated[i + 1] = segment.point(pt)
		return interpolated

	def complex2coord(self, complexnum):
		return (complexnum.real, complexnum.imag)

	def linearize(self, path, n_interpolate = 10):
		segmenttype2func = {'CubicBezier': self.linearize_curve,
				    'Line': self.linearize_line,
				    'QuadraticBezier': self.linearize_curve,
				    'Arc': self.linearize_curve}
		'''
		More sophisticated linearization option
		compared to endpts2line().
		Turn svg path into discrete coordinates
		with number of coordinates per curve set
		by n_interpolate. i.e. if n_interpolate
		is 100, each curve is approximated by
		100 discrete lines.
		'''
		segments = path._segments
		complex_coords = list()
		for segment in segments:
			# Output coordinates for each segment, minus last point (because
			# point is same as first point of next segment)
			segment_type = type(segment).__name__
			segment_linearize = segmenttype2func[segment_type]
			linearized = segment_linearize(segment, n_interpolate)
			complex_coords.extend(linearized[:-1])
		# Append last point of final segment to close the polygon
		complex_coords.append(linearized[-1])
		return [self.complex2coord(complexnum) for complexnum in complex_coords] # Output list of (x, y) tuples
