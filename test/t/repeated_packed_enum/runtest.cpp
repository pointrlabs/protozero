
#include <test.hpp>

TEST_CASE("read repeated packed enum field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_enum/data-empty");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_enum/data-one");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_enum();
        REQUIRE(!item.next());

        REQUIRE(it_pair.first != it_pair.second);
        REQUIRE(*it_pair.first == 0 /* BLACK */);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_enum/data-many");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_enum();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(it != it_pair.second);
        REQUIRE(*it++ == 0 /* BLACK */);
        REQUIRE(*it++ == 3 /* BLUE */);
        REQUIRE(*it++ == 2 /* GREEN */);
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_enum/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.packed_enum(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed enum field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("empty") {
        int32_t data[] = { 0 /* BLACK */ };
        pw.add_packed_enum(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_enum/data-empty"));
    }

    SECTION("one") {
        int32_t data[] = { 0 /* BLACK */ };
        pw.add_packed_enum(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_enum/data-one"));
    }

    SECTION("many") {
        int32_t data[] = { 0 /* BLACK */, 3 /* BLUE */, 2 /* GREEN */ };
        pw.add_packed_enum(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_enum/data-many"));
    }

}

