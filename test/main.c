
extern int test_common();

extern int test_allocator();

extern int test_array();
extern int test_hash();

extern int test_table_log();
extern int test_hash_table();

int main( int argc, char * argv[] )
{
    int result = 0;

    //result |= test_common();

    result |= test_allocator();

    //result |= test_array();
    //result |= test_hash();

    //result |= test_table_log();
    //result |= test_hash_table();

    return result;
}
