Gem::Specification.new do |s|
  f = File.new("gem_version", "r")
  s.version     = f.readline
  s.date        = "2014-03-20"
  s.name        = "genmodel"
  s.summary     = "GenModel Gem"
  s.description = "A generic tool to solve optimization model with different solvers"
  s.authors     = ["Mathieu Bouchard"]
  s.email       = 'mathbouchard@gmail.com'
  s.files       = ["ext/Genmodel/GenModelBase.cpp",
                   "ext/Genmodel/GenModel.h",
                   "ext/Genmodel/GenModelCplex.cpp",
                   "ext/Genmodel/GenModelCplex.h",
                   "ext/Genmodel/GenModelOsi.cpp",
                   "ext/Genmodel/GenModelOsi.h",
                   "ext/Genmodel/ProblemReaderOsi.cpp",
                   "ext/Genmodel/ProblemReaderOsi.h",
                   "ext/Genmodel/GenModelOsiInterface.cpp",
                   "ext/Genmodel/GenModelOsiInterface.h",
                   "ext/Genmodel/Genmodel.cpp",
                   "lib/Genmodel.rb"]
  s.homepage    = 'https://github.com/mathbouchard/GenModelGem'
  s.license     = 'MIT'
  s.extensions  = %w[ext/Genmodel/extconf.rb]
end