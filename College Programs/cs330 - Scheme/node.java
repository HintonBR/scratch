import tree;

class node extends tree {
        public String field1;
        public tree field2;
        public tree field3;
        public node(String param1, tree param2, tree param3) {
                this.field1 = param1;
                this.field2 = param2;
                this.field3 = param3;
        }
}
