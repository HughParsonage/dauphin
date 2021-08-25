library(dauphin)
dauphin_mobile1 <- function(x) dauphin_mobile(x, ignore_calling_code = TRUE)

expect_equal(dauphin_mobile1("0401 234 234"), 401234234)
expect_equal(dauphin_mobile1("0401234234"), 401234234)
expect_equal(dauphin_mobile1("401234234"), 401234234)
expect_equal(dauphin_mobile1("61401 234 234"), 401234234)
expect_equal(dauphin_mobile1("+61401 234 234"), 401234234)
expect_equal(dauphin_mobile1("+61 401 234 234"), 401234234)
expect_equal(dauphin_mobile1("+61401234234"), 401234234)
expect_equal(dauphin_mobile1("+610401234234"), 401234234)
expect_equal(dauphin_mobile1("+61 0401234234"), 401234234)
expect_equal(dauphin_mobile1(c("+61401234234",
                               "0444 444 444",
                               NA_character_,
                               "03 1234 5678"))[1:3],
             c(401234234,
               444444444,
               NA,
               NA)[1:3])
expect_equal(dauphin_mobile1("61444 999 888"),
             dauphin_mobile1( "0444 999 888"))
expect_equal(dauphin_mobile1("BOB: 61444 999 888"),
             dauphin_mobile1( "0444 999 888"))



expect_equal(dauphin_mobile_landline("0400 000 000", "(03)9876 5432", 2L),
             list(400e6, 398765432))

expect_equal(dauphin_mobile_landline("0400 000 000/0365432100", "(03)9876 5432", 2L),
             list(400e6, 398765432))
# Reverse
  expect_equal(dauphin_mobile_landline("(03)9876 5432", "0400 000 000/XX", 2L),
             list(400e6, 398765432))


