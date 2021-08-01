library(dauphin)
dauphin_mobile1 <- function(x) dauphin_mobile(x, ignore_calling_code = TRUE)

expect_equal(dauphin_mobile1("0401 234 234"), 401234234)
expect_equal(dauphin_mobile1("61401 234 234"), 401234234)
expect_equal(dauphin_mobile1("+61401 234 234"), 401234234)
expect_equal(dauphin_mobile1("+61401234234"), 401234234)
expect_equal(dauphin_mobile1(c("+61401234234",
                               "0444 444 444",
                               NA_character_,
                               "03 1234 5678"))[1:3],
             c(401234234,
               444444444,
               NA,
               NA)[1:3])
