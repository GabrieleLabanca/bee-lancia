# R CMD BATCH "--args arg1 arg2" foo.R &
voltage <- read.table(arg1)
dimension <- dim(voltagee)
time <- list(1:dimension[1])

out_name <- paste(".", gsub("\\.dat",".png",arg1), sep="/")

png(filename=out_name)
plot(voltage,time)
