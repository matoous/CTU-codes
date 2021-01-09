package kbetls

import "math/big"

// code obtained from https://stackoverflow.com/questions/31238262/is-there-a-go-function-for-obtaining-the-cube-root-of-a-big-integer

var (
	n0  = big.NewInt(0)
	n1  = big.NewInt(1)
	n2  = big.NewInt(2)
	n3  = big.NewInt(3)
	n10 = big.NewInt(10)
)

func CubeRoot(i *big.Int) (cbrt *big.Int, rem *big.Int) {
	var (
		guess = new(big.Int).Div(i, n2)
		dx    = new(big.Int)
		absDx = new(big.Int)
		minDx = new(big.Int).Abs(i)
		step  = new(big.Int).Abs(new(big.Int).Div(guess, n2))
		cube  = new(big.Int)
	)
	for {
		cube.Exp(guess, n3, nil)
		dx.Sub(i, cube)
		cmp := dx.Cmp(n0)
		if cmp == 0 {
			return guess, n0
		}

		absDx.Abs(dx)
		switch absDx.Cmp(minDx) {
		case -1:
			minDx.Set(absDx)
		case 0:
			return guess, dx
		}

		switch cmp {
		case -1:
			guess.Sub(guess, step)
		case +1:
			guess.Add(guess, step)
		}

		step.Div(step, n2)
		if step.Cmp(n0) == 0 {
			step.Set(n1)
		}
	}
}
