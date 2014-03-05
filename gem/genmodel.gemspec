Gem::Specification.new do |s|
  f = File.new("gem/gem_version", "r")
  s.version     = f.readline
  s.date        = "#{Time.now.strftime('%Y-%m-%d')}"
  s.name        = 'genmodel'
  s.summary     = "GenModel Gem"
  s.description = "A generic tool to solve optimization model with different solvers"
  s.authors     = ["Mathieu Bouchard"]
  s.email       = 'mathbouchard@gmail.com'
  s.files       = ["lib/genmodel.so", "lib/genmodel.bundle"]
  s.homepage    = 'https://github.com/mathbouchard/GenModel'
  s.license       = 'MIT'
end
