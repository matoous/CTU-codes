package kbetls

import (
	"crypto/dsa"
	"math"
	"math/big"
)

func RecoverPrivateKey() *big.Int {
	p, _ := new(big.Int).SetString("800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65eac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1", 16)
	q, _ := new(big.Int).SetString("f4f47f05794b256174bba6e9b396a7707e563c5b", 16)
	g, _ := new(big.Int).SetString("5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa4046c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291", 16)
	y, _ := new(big.Int).SetString("33ff14f19fa9cf09b28747cdfe97252c4be46c9c4c2ee68a2231cb4b262dd839962eff659bd30f706e6cb2470117f211eadfadeac267bc4fecde6d4c058cdf5d7b8c75ba663ce7a87d22b171413b8d3b6ceee31b139051c385a06b8b2e2e587a15e87381c93f866bf7b122fda5c1f44d20480137906ed6026ed96c3793fde263", 16)
	r, _ := new(big.Int).SetString("548099063082341131477253921760299949438196259240", 10)
	s, _ := new(big.Int).SetString("857042759984254168557880549501802188789837994940", 10)
	sha1, _ := new(big.Int).SetString("2bc546792a7624fb6e972b0fb85081fd20a8a28", 16)
	for ki := int64(0); ki < int64(math.Pow(2, 16)); ki++ {
		k := new(big.Int).SetInt64(ki)
		dsa.Verify(&dsa.PublicKey{
			Parameters: dsa.Parameters{
				P: p,
				Q: q,
				G: g,
			},
			Y: y,
		}, sha1.Bytes(), r, s)
		if r.Cmp(new(big.Int).Mod(new(big.Int).Exp(g, k, p), q)) == 0 {

			res := new(big.Int).Mul(new(big.Int).Sub(new(big.Int).Mul(s, k), sha1), new(big.Int).ModInverse(r, q))
			x := new(big.Int).Mod(res, q)
			return x
		}
	}
	return nil
}
