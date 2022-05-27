
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

/*
  Declaraytions
*/

/// Helper declarations

/// String declarations

char* CreateString( const char* value );
void FreeString( char** stringPtr );

unsigned AppendString( char** destString, const char* value );

/// File Path declarations

unsigned SplitPath( const char* fromFullPath, char** toDir,
  char** toBaseName, char** toExt );
unsigned JoinPath( const char* fromDir, const char* fromBaseName,
  const char* fromExt, char** toFullPath );

/// Symbol Table declarations

typedef struct SymbolTable {
} SymbolTable;

SymbolTable* CreateSymbolTable();
void ReleaseSymbolTable( SymbolTable** symbolTablePtr );

unsigned DeclareNamespace( SymbolTable* symbolTable,
  const char* namespace );

/// Parser deckarations

#define NOTHINGTOSKIP 0
#define SKIPPED 2

typedef struct Parser {
  FILE* handle;

  int ch;

  unsigned line;
  unsigned column;

  int nextch;

  unsigned nextLine;
  unsigned nextColumn;

  unsigned entryPoint;
} Parser;

Parser* OpenGrammar( const char* fileName );
void CloseGrammar( Parser* grammar );
void ReleaseGrammar( Parser** grammarPtr );

int ReadChar( Parser* grammar );

unsigned SkipSpace( Parser* grammar );
unsigned SkipCommentsAndSpace( Parser* grammar );

unsigned MarkParserPosition( Parser* grammar );
unsigned RestoreParserPosition( Parser* grammar );

unsigned ReadIdent( Parser* grammar, char** identPtr );

unsigned ParseGrammar( Parser* grammar, SymbolTable* symbolTable,
  struct Compiler* compiler );

/// Compiler declarations

typedef struct Compiler {
  FILE* sourceHandle;
  FILE* importHandle;
  char* outCode;
} Compiler;

Compiler* CreateCompiler( const char* sourceFileName,
  const char* importFileName );
void CloseCompiler( Compiler* compiler );
void ReleaseCompiler( Compiler** compilerPtr );

int BuildCompiler( const char* sourcefileName,
  const char* exeFileName, const char* importFileName );

int PatchVersion( const char* fileName,
  const char* version, const char* versionstr,
  const char* description, const char* copyright );

/*
  Main program
*/

char* compilerCopyright = NULL;
char* compilerDescription = NULL;
char* compilerVersion = NULL;
char* compilerVersionStr = NULL;

char* grammarPath = NULL;
char* grammarBaseName = NULL;
char* grammarExt = NULL;

char* compilerPath = NULL;
char* compilerBaseName = NULL;
char* compilerExt = NULL;

char* grammarFileName = NULL;
char* compilerSourceFileName = NULL;
char* compilerImportFileName = NULL;
char* compilerExeFileName = NULL;

Parser* parser = NULL;
SymbolTable* symtab = NULL;
Compiler* compiler = NULL;

char* ident = NULL;

void ShowBanner() {
  printf( "Origo compiler generator v%u.%u %s\n"
    "Copyright 2022 Orlando Llanes\n\n", 0, 1, "Alpha" );
}

void ShowUsage() {
  printf(
    "Usage:\n"
    "origo -new outfile[.ocg]\n"
    "origo infile[.ocg] [outfile[.exe]]\n" );
}

void Cleanup() {
  FreeString( &compilerCopyright );
  FreeString( &compilerDescription );
  FreeString( &compilerVersion );
  FreeString( &compilerVersionStr );

  FreeString( &grammarPath );
  FreeString( &grammarBaseName );
  FreeString( &grammarExt );

  FreeString( &compilerPath );
  FreeString( &compilerBaseName );
  FreeString( &compilerExt );

  FreeString( &grammarFileName );
  FreeString( &compilerSourceFileName );
  FreeString( &compilerImportFileName );
  FreeString( &compilerExeFileName );

  ReleaseGrammar( &parser );
  ReleaseSymbolTable( &symtab );
  ReleaseCompiler( &compiler );

  FreeString( &ident );
}

void InferFileNames( int argc, char** argv[] ) {
  unsigned result = 0;

  switch( argc ) {
  case 2:
    if( result = SplitPath((const char*)argv[1], &grammarPath,
        &grammarBaseName, &grammarExt) ) {
      printf( "Internal error: %u\n", result );
      exit(2);
    }

    compilerPath = CreateString(grammarPath);
    compilerBaseName = CreateString(grammarBaseName);
    break;

  case 3:
    if( result = SplitPath((const char*)argv[1], &grammarPath,
        &grammarBaseName, &grammarExt) ) {
      printf( "Internal error: %u\n", result );
      exit(2);
    }

    if( result = SplitPath((const char*)argv[2], &compilerPath,
        &compilerBaseName, &compilerExt) ) {
      printf( "Internal error: %u\n", result );
      exit(2);
    }
    break;

  default:
    ShowUsage();
    exit(2);
  }

  if( grammarExt == NULL ) {
    grammarExt = CreateString(".ocg");
    if( grammarExt == NULL ) {
      printf( "Internal error.\n" );
      exit(2);
    }
  }

  if( compilerExt == NULL ) {
    compilerExt = CreateString(".exe");
    if( compilerExt == NULL ) {
      printf( "Internal error.\n" );
      exit(2);
    }
  }

  if( result = JoinPath(grammarPath, grammarBaseName, grammarExt,
      &grammarFileName) ) {
    printf( "Internal error: %u\n", result );
    exit(2);
  }

  if( result = JoinPath(compilerPath, compilerBaseName, ".c",
      &compilerSourceFileName) ) {
    printf( "Internal error: %u\n", result );
    exit(2);
  }

  if( result = JoinPath(compilerPath, compilerBaseName, ".def",
      &compilerImportFileName) ) {
    printf( "Internal error: %u\n", result );
    exit(2);
  }

  if( result = JoinPath(compilerPath, compilerBaseName, compilerExt,
      &compilerExeFileName) ) {
    printf( "Internal error: %s\n", result );
    exit(2);
  }
}

static const char* genGrammarText = {
  "\n"
  "/*\n"
  " * Begin Optional overrides - Uncomment // to change.\n"
  " */\n"
  "\n"
  "/* Written to the executable, and shown on invocation. */\n"
  "// copyright = \"(C) YEAR NAME/ORGANIZATION\";\n"
  "\n"
  "/* Written to the executable */\n"
  "// description = \"XYZ compiler\";\n"
  "\n"
  "/* Written to the executable */\n"
  "// version = \"x.x.x.x\";\n"
  "\n"
  "/* Written to the executable, and shown on invocation. */\n"
  "// versionstr = \"x.x.x.x STAGE\"\n"
  "\n"
  "/* Changes default identifier definition */\n"
  "// ident = _a/zA/Z{_a/zA/Z0/9};\n"
  "\n"
  "/*\n"
  " * End optional overrides.\n"
  " */\n"
  "\n"
  "/*\n"
  " * Parse rule reference:\n"
/// TODO: Add pre-defined tag reference
  " */\n"
  "\n"
  "/*\n"
  " * Begin pre-defined parser starting point (required).\n"
  " */\n"
  "parse = /* RULE */;\n"
  "\n"
  "/*\n"
  " * End pre-defined parser starting point.\n"
  " */\n"
  "\n"
  "/* Custom parsing rules. */\n"
  "// RULENAME = RULE;\n"
  "\n"
};


void ParseOptions( int argc, char** argv ) {
  char* genGrammarPath = NULL;
  char* genGrammarBaseName = NULL;
  char* genGrammarExt = NULL;
  char* genGrammarFileName = NULL;
  FILE* genGrammarFile = NULL;

  /// TODO: Move new file generation to its own function
  switch( argc ) {
  case 2:
    if( strcmp(argv[1], "-new") == 0 ) {
      ShowUsage();
    }
    exit(3);

  case 3:
    if( argv && argv[1] && (strcmp(argv[1], "-new") == 0) ) {
      if( argv[2] == NULL ) {
        printf( "Internal error.\n" );
        exit(3);
      }

      if( SplitPath(argv[2],
          &genGrammarPath, &genGrammarBaseName, &genGrammarExt) == 0 ) {

        if( genGrammarExt == NULL ) {
          genGrammarExt = CreateString(".ocg");
        }

        JoinPath(genGrammarPath, genGrammarBaseName, genGrammarExt,
          &genGrammarFileName);

        FreeString( &grammarPath );
        FreeString( &grammarBaseName );
        FreeString( &grammarExt );

        if( genGrammarFileName == NULL ) {
          printf( "Internal error.\n" );
          exit(3);
        }

        genGrammarFile = fopen(genGrammarFileName, "w");
        if( genGrammarFile == NULL ) {
          printf( "Error generating new grammar file '%s'\n", genGrammarFileName );
          FreeString( &genGrammarFileName);
          exit(3);
        }

        FreeString( &genGrammarFileName);

        fprintf( genGrammarFile, "%s", genGrammarText );

        fclose( genGrammarFile );
        genGrammarFile = NULL;

        exit(0);
      }
    }
    printf( "Internal error\n" );
    exit(3);
  }

  InferFileNames( argc, argv );
}

int main( int argc, char** argv ) {
  int result = 0;

  atexit( Cleanup );

  ShowBanner();

  ParseOptions( argc, argv );

  parser = OpenGrammar(grammarFileName);
  if( parser == NULL ) {
    printf( "Error opening source grammar '%s'\n", grammarFileName );
    exit(1);
  }

  compiler = CreateCompiler(compilerSourceFileName,
    compilerImportFileName);
  if( compiler == NULL ) {
    printf( "Error creating compiler '%s' '%s'\n",
      compilerSourceFileName, compilerImportFileName );
    exit(1);
  }

  result = ParseGrammar(parser, symtab, compiler);
  if( result ) {
    printf( "Internal error: %u\n", result );
    exit(1);
  }

  CloseGrammar( parser );
  CloseCompiler( compiler );

  result = BuildCompiler(compilerSourceFileName, compilerExeFileName,
    compilerImportFileName);

  result = PatchVersion(compilerExeFileName,
    compilerVersion, compilerVersionStr,
    compilerDescription, compilerCopyright);

  Cleanup();

  return 0;
}

/*
  Inline dependencies
 */

/*
  Implementations
*/

#include <process.h> // For: _spawnvp

/// BEGIN INLINE COPY OF AVL_TREE.H BY ERIC BIGGERS
/*
 * avl_tree.h - intrusive, nonrecursive AVL tree data structure (self-balancing
 *		binary search tree), header file
 *
 * Written in 2014-2016 by Eric Biggers <...>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide via the Creative Commons Zero 1.0 Universal Public Domain
 * Dedication (the "CC0").
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the CC0 for more details.
 *
 * You should have received a copy of the CC0 along with this software; if not
 * see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h> /* for uintptr_t */

#ifdef __GNUC__
#  define AVL_INLINE inline __attribute__((always_inline))
#else
#  define AVL_INLINE inline
//#  warning "AVL tree functions may not be inlined as intended"
#endif

/* Node in an AVL tree.  Embed this in some other data structure.  */
struct avl_tree_node {

	/* Pointer to left child or NULL  */
	struct avl_tree_node *left;

	/* Pointer to right child or NULL  */
	struct avl_tree_node *right;

	/* Pointer to parent combined with the balance factor.  This saves 4 or
	 * 8 bytes of memory depending on the CPU architecture.
	 *
	 * Low 2 bits:  One greater than the balance factor of this subtree,
	 * which is equal to height(right) - height(left).  The mapping is:
	 *
	 * 00 => -1
	 * 01 =>  0
	 * 10 => +1
	 * 11 => undefined
	 *
	 * The rest of the bits are the pointer to the parent node.  It must be
	 * 4-byte aligned, and it will be NULL if this is the root node and
	 * therefore has no parent.  */
	uintptr_t parent_balance;
};

/* Cast an AVL tree node to the containing data structure.  */
#define avl_tree_entry(entry, type, member) \
	((type*) ((char *)(entry) - offsetof(type, member)))

/* Returns a pointer to the parent of the specified AVL tree node, or NULL if it
 * is already the root of the tree.  */
static AVL_INLINE struct avl_tree_node *
avl_get_parent(const struct avl_tree_node *node)
{
	return (struct avl_tree_node *)(node->parent_balance & ~3);
}

/* Marks the specified AVL tree node as unlinked from any tree.  */
static AVL_INLINE void
avl_tree_node_set_unlinked(struct avl_tree_node *node)
{
	node->parent_balance = (uintptr_t)node;
}

/* Returns true iff the specified AVL tree node has been marked with
 * avl_tree_node_set_unlinked() and has not subsequently been inserted into a
 * tree.  */
static AVL_INLINE bool
avl_tree_node_is_unlinked(const struct avl_tree_node *node)
{
	return node->parent_balance == (uintptr_t)node;
}

/* (Internal use only)  */
extern void
avl_tree_rebalance_after_insert(struct avl_tree_node **root_ptr,
				struct avl_tree_node *inserted);

/*
 * Looks up an item in the specified AVL tree.
 *
 * @root
 *	Pointer to the root of the AVL tree.  (This can be NULL --- that just
 *	means the tree is empty.)
 *
 * @cmp_ctx
 *	First argument to pass to the comparison callback.  This generally
 *	should be a pointer to an object equal to the one being searched for.
 *
 * @cmp
 *	Comparison callback.  Must return < 0, 0, or > 0 if the first argument
 *	is less than, equal to, or greater than the second argument,
 *	respectively.  The first argument will be @cmp_ctx and the second
 *	argument will be a pointer to the AVL tree node of an item in the tree.
 *
 * Returns a pointer to the AVL tree node of the resulting item, or NULL if the
 * item was not found.
 *
 * Example:
 *
 * struct int_wrapper {
 *	int data;
 *	struct avl_tree_node index_node;
 * };
 *
 * static int _avl_cmp_int_to_node(const void *intptr,
 *				   const struct avl_tree_node *nodeptr)
 * {
 *	int n1 = *(const int *)intptr;
 *	int n2 = avl_tree_entry(nodeptr, struct int_wrapper, index_node)->data;
 *	if (n1 < n2)
 *		return -1;
 *	else if (n1 > n2)
 *		return 1;
 *	else
 *		return 0;
 * }
 *
 * bool contains_int(struct avl_tree_node *root, int n)
 * {
 *	struct avl_tree_node *result;
 *
 *	result = avl_tree_lookup(root, &n, _avl_cmp_int_to_node);
 *	return result ? true : false;
 * }
 */
static AVL_INLINE struct avl_tree_node *
avl_tree_lookup(const struct avl_tree_node *root,
		const void *cmp_ctx,
		int (*cmp)(const void *, const struct avl_tree_node *))
{
	const struct avl_tree_node *cur = root;

	while (cur) {
		int res = (*cmp)(cmp_ctx, cur);
		if (res < 0)
			cur = cur->left;
		else if (res > 0)
			cur = cur->right;
		else
			break;
	}
	return (struct avl_tree_node*)cur;
}

/* Same as avl_tree_lookup(), but uses a more specific type for the comparison
 * function.  Specifically, with this function the item being searched for is
 * expected to be in the same format as those already in the tree, with an
 * embedded 'struct avl_tree_node'.  */
static AVL_INLINE struct avl_tree_node *
avl_tree_lookup_node(const struct avl_tree_node *root,
		     const struct avl_tree_node *node,
		     int (*cmp)(const struct avl_tree_node *,
				const struct avl_tree_node *))
{
	const struct avl_tree_node *cur = root;

	while (cur) {
		int res = (*cmp)(node, cur);
		if (res < 0)
			cur = cur->left;
		else if (res > 0)
			cur = cur->right;
		else
			break;
	}
	return (struct avl_tree_node*)cur;
}

/*
 * Inserts an item into the specified AVL tree.
 *
 * @root_ptr
 *	Location of the AVL tree's root pointer.  Indirection is needed because
 *	the root node may change as a result of rotations caused by the
 *	insertion.  Initialize *root_ptr to NULL for an empty tree.
 *
 * @item
 *	Pointer to the `struct avl_tree_node' embedded in the item to insert.
 *	No members in it need be pre-initialized, although members in the
 *	containing structure should be pre-initialized so that @cmp can use them
 *	in comparisons.
 *
 * @cmp
 *	Comparison callback.  Must return < 0, 0, or > 0 if the first argument
 *	is less than, equal to, or greater than the second argument,
 *	respectively.  The first argument will be @item and the second
 *	argument will be a pointer to an AVL tree node embedded in some
 *	previously-inserted item to which @item is being compared.
 *
 * If no item in the tree is comparatively equal (via @cmp) to @item, inserts
 * @item and returns NULL.  Otherwise does nothing and returns a pointer to the
 * AVL tree node embedded in the previously-inserted item which compared equal
 * to @item.
 *
 * Example:
 *
 * struct int_wrapper {
 *	int data;
 *	struct avl_tree_node index_node;
 * };
 *
 * #define GET_DATA(i) avl_tree_entry((i), struct int_wrapper, index_node)->data
 *
 * static int _avl_cmp_ints(const struct avl_tree_node *node1,
 *			    const struct avl_tree_node *node2)
 * {
 *	int n1 = GET_DATA(node1);
 *	int n2 = GET_DATA(node2);
 *	if (n1 < n2)
 *		return -1;
 *	else if (n1 > n2)
 *		return 1;
 *	else
 *		return 0;
 * }
 *
 * bool insert_int(struct avl_tree_node **root_ptr, int data)
 * {
 *	struct int_wrapper *i = malloc(sizeof(struct int_wrapper));
 *	i->data = data;
 *	if (avl_tree_insert(root_ptr, &i->index_node, _avl_cmp_ints)) {
 *		// Duplicate.
 *		free(i);
 *		return false;
 *	}
 *	return true;
 * }
 */
static AVL_INLINE struct avl_tree_node *
avl_tree_insert(struct avl_tree_node **root_ptr,
		struct avl_tree_node *item,
		int (*cmp)(const struct avl_tree_node *,
			   const struct avl_tree_node *))
{
	struct avl_tree_node **cur_ptr = root_ptr, *cur = NULL;
	int res;

	while (*cur_ptr) {
		cur = *cur_ptr;
		res = (*cmp)(item, cur);
		if (res < 0)
			cur_ptr = &cur->left;
		else if (res > 0)
			cur_ptr = &cur->right;
		else
			return cur;
	}
	*cur_ptr = item;
	item->parent_balance = (uintptr_t)cur | 1;
	avl_tree_rebalance_after_insert(root_ptr, item);
	return NULL;
}

/* Removes an item from the specified AVL tree.
 * See implementation for details.  */
extern void
avl_tree_remove(struct avl_tree_node **root_ptr, struct avl_tree_node *node);

/* Nonrecursive AVL tree traversal functions  */

extern struct avl_tree_node *
avl_tree_first_in_order(const struct avl_tree_node *root);

extern struct avl_tree_node *
avl_tree_last_in_order(const struct avl_tree_node *root);

extern struct avl_tree_node *
avl_tree_next_in_order(const struct avl_tree_node *node);

extern struct avl_tree_node *
avl_tree_prev_in_order(const struct avl_tree_node *node);

extern struct avl_tree_node *
avl_tree_first_in_postorder(const struct avl_tree_node *root);

extern struct avl_tree_node *
avl_tree_next_in_postorder(const struct avl_tree_node *prev,
			   const struct avl_tree_node *prev_parent);

/*
 * Iterate through the nodes in an AVL tree in sorted order.
 * You may not modify the tree during the iteration.
 *
 * @child_struct
 *	Variable that will receive a pointer to each struct inserted into the
 *	tree.
 * @root
 *	Root of the AVL tree.
 * @struct_name
 *	Type of *child_struct.
 * @struct_member
 *	Member of @struct_name type that is the AVL tree node.
 *
 * Example:
 *
 * struct int_wrapper {
 *	int data;
 *	struct avl_tree_node index_node;
 * };
 *
 * void print_ints(struct avl_tree_node *root)
 * {
 *	struct int_wrapper *i;
 *
 *	avl_tree_for_each_in_order(i, root, struct int_wrapper, index_node)
 *		printf("%d\n", i->data);
 * }
 */
#define avl_tree_for_each_in_order(child_struct, root,			\
				   struct_name, struct_member)		\
	for (struct avl_tree_node *_cur =				\
		avl_tree_first_in_order(root);				\
	     _cur && ((child_struct) =					\
		      avl_tree_entry(_cur, struct_name,			\
				     struct_member), 1);		\
	     _cur = avl_tree_next_in_order(_cur))

/*
 * Like avl_tree_for_each_in_order(), but uses the reverse order.
 */
#define avl_tree_for_each_in_reverse_order(child_struct, root,		\
					   struct_name, struct_member)	\
	for (struct avl_tree_node *_cur =				\
		avl_tree_last_in_order(root);				\
	     _cur && ((child_struct) =					\
		      avl_tree_entry(_cur, struct_name,			\
				     struct_member), 1);		\
	     _cur = avl_tree_prev_in_order(_cur))

/*
 * Like avl_tree_for_each_in_order(), but iterates through the nodes in
 * postorder, so the current node may be deleted or freed.
 */
#define avl_tree_for_each_in_postorder(child_struct, root,		\
				       struct_name, struct_member)	\
	for (struct avl_tree_node *_cur =				\
		avl_tree_first_in_postorder(root), *_parent;		\
	     _cur && ((child_struct) =					\
		      avl_tree_entry(_cur, struct_name,			\
				     struct_member), 1)			\
	          && (_parent = avl_get_parent(_cur), 1);		\
	     _cur = avl_tree_next_in_postorder(_cur, _parent))

#endif /* _AVL_TREE_H_ */
/// END INLINE COPY OF AVL_TREE.H BY ERIC BIGGERS

/// BEGIN INLINE COPY OF AVL_TREE.C BY ERIC BIGGERS
/*
 * avl_tree.c - intrusive, nonrecursive AVL tree data structure (self-balancing
 *		binary search tree), implementation file
 *
 * Written in 2014-2016 by Eric Biggers <...>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide via the Creative Commons Zero 1.0 Universal Public Domain
 * Dedication (the "CC0").
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the CC0 for more details.
 *
 * You should have received a copy of the CC0 along with this software; if not
 * see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

//#include "avl_tree.h"

/* Returns the left child (sign < 0) or the right child (sign > 0) of the
 * specified AVL tree node.
 * Note: for all calls of this, 'sign' is constant at compilation time,
 * so the compiler can remove the conditional.  */
static AVL_INLINE struct avl_tree_node *
avl_get_child(const struct avl_tree_node *parent, int sign)
{
	if (sign < 0)
		return parent->left;
	else
		return parent->right;
}

static AVL_INLINE struct avl_tree_node *
avl_tree_first_or_last_in_order(const struct avl_tree_node *root, int sign)
{
	const struct avl_tree_node *first = root;

	if (first)
		while (avl_get_child(first, +sign))
			first = avl_get_child(first, +sign);
	return (struct avl_tree_node *)first;
}

/* Starts an in-order traversal of the tree: returns the least-valued node, or
 * NULL if the tree is empty.  */
struct avl_tree_node *
avl_tree_first_in_order(const struct avl_tree_node *root)
{
	return avl_tree_first_or_last_in_order(root, -1);
}

/* Starts a *reverse* in-order traversal of the tree: returns the
 * greatest-valued node, or NULL if the tree is empty.  */
struct avl_tree_node *
avl_tree_last_in_order(const struct avl_tree_node *root)
{
	return avl_tree_first_or_last_in_order(root, 1);
}

static AVL_INLINE struct avl_tree_node *
avl_tree_next_or_prev_in_order(const struct avl_tree_node *node, int sign)
{
	const struct avl_tree_node *next;

	if (avl_get_child(node, +sign))
		for (next = avl_get_child(node, +sign);
		     avl_get_child(next, -sign);
		     next = avl_get_child(next, -sign))
			;
	else
		for (next = avl_get_parent(node);
		     next && node == avl_get_child(next, +sign);
		     node = next, next = avl_get_parent(next))
			;
	return (struct avl_tree_node *)next;
}

/* Continues an in-order traversal of the tree: returns the next-greatest-valued
 * node, or NULL if there is none.  */
struct avl_tree_node *
avl_tree_next_in_order(const struct avl_tree_node *node)
{
	return avl_tree_next_or_prev_in_order(node, 1);
}

/* Continues a *reverse* in-order traversal of the tree: returns the
 * previous-greatest-valued node, or NULL if there is none.  */
struct avl_tree_node *
avl_tree_prev_in_order(const struct avl_tree_node *node)
{
	return avl_tree_next_or_prev_in_order(node, -1);
}

/* Starts a postorder traversal of the tree.  */
struct avl_tree_node *
avl_tree_first_in_postorder(const struct avl_tree_node *root)
{
	const struct avl_tree_node *first = root;

	if (first)
		while (first->left || first->right)
			first = first->left ? first->left : first->right;

	return (struct avl_tree_node *)first;
}

/* Continues a postorder traversal of the tree.  @prev will not be deferenced as
 * it's allowed that its memory has been freed; @prev_parent must be its saved
 * parent node.  Returns NULL if there are no more nodes (i.e. @prev was the
 * root of the tree).  */
struct avl_tree_node *
avl_tree_next_in_postorder(const struct avl_tree_node *prev,
			   const struct avl_tree_node *prev_parent)
{
	const struct avl_tree_node *next = prev_parent;

	if (next && prev == next->left && next->right)
		for (next = next->right;
		     next->left || next->right;
		     next = next->left ? next->left : next->right)
			;
	return (struct avl_tree_node *)next;
}

/* Sets the left child (sign < 0) or the right child (sign > 0) of the
 * specified AVL tree node.
 * Note: for all calls of this, 'sign' is constant at compilation time,
 * so the compiler can remove the conditional.  */
static AVL_INLINE void
avl_set_child(struct avl_tree_node *parent, int sign,
	      struct avl_tree_node *child)
{
	if (sign < 0)
		parent->left = child;
	else
		parent->right = child;
}

/* Sets the parent and balance factor of the specified AVL tree node.  */
static AVL_INLINE void
avl_set_parent_balance(struct avl_tree_node *node, struct avl_tree_node *parent,
		       int balance_factor)
{
	node->parent_balance = (uintptr_t)parent | (balance_factor + 1);
}

/* Sets the parent of the specified AVL tree node.  */
static AVL_INLINE void
avl_set_parent(struct avl_tree_node *node, struct avl_tree_node *parent)
{
	node->parent_balance = (uintptr_t)parent | (node->parent_balance & 3);
}

/* Returns the balance factor of the specified AVL tree node --- that is, the
 * height of its right subtree minus the height of its left subtree.  */
static AVL_INLINE int
avl_get_balance_factor(const struct avl_tree_node *node)
{
	return (int)(node->parent_balance & 3) - 1;
}

/* Adds @amount to the balance factor of the specified AVL tree node.
 * The caller must ensure this still results in a valid balance factor
 * (-1, 0, or 1).  */
static AVL_INLINE void
avl_adjust_balance_factor(struct avl_tree_node *node, int amount)
{
	node->parent_balance += amount;
}

static AVL_INLINE void
avl_replace_child(struct avl_tree_node **root_ptr,
		  struct avl_tree_node *parent,
		  struct avl_tree_node *old_child,
		  struct avl_tree_node *new_child)
{
	if (parent) {
		if (old_child == parent->left)
			parent->left = new_child;
		else
			parent->right = new_child;
	} else {
		*root_ptr = new_child;
	}
}

/*
 * Template for performing a single rotation ---
 *
 * sign > 0:  Rotate clockwise (right) rooted at A:
 *
 *           P?            P?
 *           |             |
 *           A             B
 *          / \           / \
 *         B   C?  =>    D?  A
 *        / \               / \
 *       D?  E?            E?  C?
 *
 * (nodes marked with ? may not exist)
 *
 * sign < 0:  Rotate counterclockwise (left) rooted at A:
 *
 *           P?            P?
 *           |             |
 *           A             B
 *          / \           / \
 *         C?  B   =>    A   D?
 *            / \       / \
 *           E?  D?    C?  E?
 *
 * This updates pointers but not balance factors!
 */
static AVL_INLINE void
avl_rotate(struct avl_tree_node ** const root_ptr,
	   struct avl_tree_node * const A, const int sign)
{
	struct avl_tree_node * const B = avl_get_child(A, -sign);
	struct avl_tree_node * const E = avl_get_child(B, +sign);
	struct avl_tree_node * const P = avl_get_parent(A);

	avl_set_child(A, -sign, E);
	avl_set_parent(A, B);

	avl_set_child(B, +sign, A);
	avl_set_parent(B, P);

	if (E)
		avl_set_parent(E, A);

	avl_replace_child(root_ptr, P, A, B);
}

/*
 * Template for performing a double rotation ---
 *
 * sign > 0:  Rotate counterclockwise (left) rooted at B, then
 *		     clockwise (right) rooted at A:
 *
 *           P?            P?          P?
 *           |             |           |
 *           A             A           E
 *          / \           / \        /   \
 *         B   C?  =>    E   C? =>  B     A
 *        / \           / \        / \   / \
 *       D?  E         B   G?     D?  F?G?  C?
 *          / \       / \
 *         F?  G?    D?  F?
 *
 * (nodes marked with ? may not exist)
 *
 * sign < 0:  Rotate clockwise (right) rooted at B, then
 *		     counterclockwise (left) rooted at A:
 *
 *         P?          P?              P?
 *         |           |               |
 *         A           A               E
 *        / \         / \            /   \
 *       C?  B   =>  C?  E    =>    A     B
 *          / \         / \        / \   / \
 *         E   D?      G?  B      C?  G?F?  D?
 *        / \             / \
 *       G?  F?          F?  D?
 *
 * Returns a pointer to E and updates balance factors.  Except for those
 * two things, this function is equivalent to:
 *	avl_rotate(root_ptr, B, -sign);
 *	avl_rotate(root_ptr, A, +sign);
 *
 * See comment in avl_handle_subtree_growth() for explanation of balance
 * factor updates.
 */
static AVL_INLINE struct avl_tree_node *
avl_do_double_rotate(struct avl_tree_node ** const root_ptr,
		     struct avl_tree_node * const B,
		     struct avl_tree_node * const A, const int sign)
{
	struct avl_tree_node * const E = avl_get_child(B, +sign);
	struct avl_tree_node * const F = avl_get_child(E, -sign);
	struct avl_tree_node * const G = avl_get_child(E, +sign);
	struct avl_tree_node * const P = avl_get_parent(A);
	const int e = avl_get_balance_factor(E);

	avl_set_child(A, -sign, G);
	avl_set_parent_balance(A, E, ((sign * e >= 0) ? 0 : -e));

	avl_set_child(B, +sign, F);
	avl_set_parent_balance(B, E, ((sign * e <= 0) ? 0 : -e));

	avl_set_child(E, +sign, A);
	avl_set_child(E, -sign, B);
	avl_set_parent_balance(E, P, 0);

	if (G)
		avl_set_parent(G, A);

	if (F)
		avl_set_parent(F, B);

	avl_replace_child(root_ptr, P, A, E);

	return E;
}

/*
 * This function handles the growth of a subtree due to an insertion.
 *
 * @root_ptr
 *	Location of the tree's root pointer.
 *
 * @node
 *	A subtree that has increased in height by 1 due to an insertion.
 *
 * @parent
 *	Parent of @node; must not be NULL.
 *
 * @sign
 *	-1 if @node is the left child of @parent;
 *	+1 if @node is the right child of @parent.
 *
 * This function will adjust @parent's balance factor, then do a (single
 * or double) rotation if necessary.  The return value will be %true if
 * the full AVL tree is now adequately balanced, or %false if the subtree
 * rooted at @parent is now adequately balanced but has increased in
 * height by 1, so the caller should continue up the tree.
 *
 * Note that if %false is returned, no rotation will have been done.
 * Indeed, a single node insertion cannot require that more than one
 * (single or double) rotation be done.
 */
static AVL_INLINE bool
avl_handle_subtree_growth(struct avl_tree_node ** const root_ptr,
			  struct avl_tree_node * const node,
			  struct avl_tree_node * const parent,
			  const int sign)
{
	int old_balance_factor, new_balance_factor;

	old_balance_factor = avl_get_balance_factor(parent);

	if (old_balance_factor == 0) {
		avl_adjust_balance_factor(parent, sign);
		/* @parent is still sufficiently balanced (-1 or +1
		 * balance factor), but must have increased in height.
		 * Continue up the tree.  */
		return false;
	}

	new_balance_factor = old_balance_factor + sign;

	if (new_balance_factor == 0) {
		avl_adjust_balance_factor(parent, sign);
		/* @parent is now perfectly balanced (0 balance factor).
		 * It cannot have increased in height, so there is
		 * nothing more to do.  */
		return true;
	}

	/* @parent is too left-heavy (new_balance_factor == -2) or
	 * too right-heavy (new_balance_factor == +2).  */

	/* Test whether @node is left-heavy (-1 balance factor) or
	 * right-heavy (+1 balance factor).
	 * Note that it cannot be perfectly balanced (0 balance factor)
	 * because here we are under the invariant that @node has
	 * increased in height due to the insertion.  */
	if (sign * avl_get_balance_factor(node) > 0) {

		/* @node (B below) is heavy in the same direction @parent
		 * (A below) is heavy.
		 *
		 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		 * The comment, diagram, and equations below assume sign < 0.
		 * The other case is symmetric!
		 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		 *
		 * Do a clockwise rotation rooted at @parent (A below):
		 *
		 *           A              B
		 *          / \           /   \
		 *         B   C?  =>    D     A
		 *        / \           / \   / \
		 *       D   E?        F?  G?E?  C?
		 *      / \
		 *     F?  G?
		 *
		 * Before the rotation:
		 *	balance(A) = -2
		 *	balance(B) = -1
		 * Let x = height(C).  Then:
		 *	height(B) = x + 2
		 *	height(D) = x + 1
		 *	height(E) = x
		 *	max(height(F), height(G)) = x.
		 *
		 * After the rotation:
		 *	height(D) = max(height(F), height(G)) + 1
		 *		  = x + 1
		 *	height(A) = max(height(E), height(C)) + 1
		 *		  = max(x, x) + 1 = x + 1
		 *	balance(B) = 0
		 *	balance(A) = 0
		 */
		avl_rotate(root_ptr, parent, -sign);

		/* Equivalent to setting @parent's balance factor to 0.  */
		avl_adjust_balance_factor(parent, -sign); /* A */

		/* Equivalent to setting @node's balance factor to 0.  */
		avl_adjust_balance_factor(node, -sign);   /* B */
	} else {
		/* @node (B below) is heavy in the direction opposite
		 * from the direction @parent (A below) is heavy.
		 *
		 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		 * The comment, diagram, and equations below assume sign < 0.
		 * The other case is symmetric!
		 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		 *
		 * Do a counterblockwise rotation rooted at @node (B below),
		 * then a clockwise rotation rooted at @parent (A below):
		 *
		 *           A             A           E
		 *          / \           / \        /   \
		 *         B   C?  =>    E   C? =>  B     A
		 *        / \           / \        / \   / \
		 *       D?  E         B   G?     D?  F?G?  C?
		 *          / \       / \
		 *         F?  G?    D?  F?
		 *
		 * Before the rotation:
		 *	balance(A) = -2
		 *	balance(B) = +1
		 * Let x = height(C).  Then:
		 *	height(B) = x + 2
		 *	height(E) = x + 1
		 *	height(D) = x
		 *	max(height(F), height(G)) = x
		 *
		 * After both rotations:
		 *	height(A) = max(height(G), height(C)) + 1
		 *		  = x + 1
		 *	balance(A) = balance(E{orig}) >= 0 ? 0 : -balance(E{orig})
		 *	height(B) = max(height(D), height(F)) + 1
		 *		  = x + 1
		 *	balance(B) = balance(E{orig} <= 0) ? 0 : -balance(E{orig})
		 *
		 *	height(E) = x + 2
		 *	balance(E) = 0
		 */
		avl_do_double_rotate(root_ptr, node, parent, -sign);
	}

	/* Height after rotation is unchanged; nothing more to do.  */
	return true;
}

/* Rebalance the tree after insertion of the specified node.  */
void
avl_tree_rebalance_after_insert(struct avl_tree_node **root_ptr,
				struct avl_tree_node *inserted)
{
	struct avl_tree_node *node, *parent;
	bool done;

	inserted->left = NULL;
	inserted->right = NULL;

	node = inserted;

	/* Adjust balance factor of new node's parent.
	 * No rotation will need to be done at this level.  */

	parent = avl_get_parent(node);
	if (!parent)
		return;

	if (node == parent->left)
		avl_adjust_balance_factor(parent, -1);
	else
		avl_adjust_balance_factor(parent, +1);

	if (avl_get_balance_factor(parent) == 0)
		/* @parent did not change in height.  Nothing more to do.  */
		return;

	/* The subtree rooted at @parent increased in height by 1.  */

	do {
		/* Adjust balance factor of next ancestor.  */

		node = parent;
		parent = avl_get_parent(node);
		if (!parent)
			return;

		/* The subtree rooted at @node has increased in height by 1.  */
		if (node == parent->left)
			done = avl_handle_subtree_growth(root_ptr, node,
							 parent, -1);
		else
			done = avl_handle_subtree_growth(root_ptr, node,
							 parent, +1);
	} while (!done);
}

/*
 * This function handles the shrinkage of a subtree due to a deletion.
 *
 * @root_ptr
 *	Location of the tree's root pointer.
 *
 * @parent
 *	A node in the tree, exactly one of whose subtrees has decreased
 *	in height by 1 due to a deletion.  (This includes the case where
 *	one of the child pointers has become NULL, since we can consider
 *	the "NULL" subtree to have a height of 0.)
 *
 * @sign
 *	+1 if the left subtree of @parent has decreased in height by 1;
 *	-1 if the right subtree of @parent has decreased in height by 1.
 *
 * @left_deleted_ret
 *	If the return value is not NULL, this will be set to %true if the
 *	left subtree of the returned node has decreased in height by 1,
 *	or %false if the right subtree of the returned node has decreased
 *	in height by 1.
 *
 * This function will adjust @parent's balance factor, then do a (single
 * or double) rotation if necessary.  The return value will be NULL if
 * the full AVL tree is now adequately balanced, or a pointer to the
 * parent of @parent if @parent is now adequately balanced but has
 * decreased in height by 1.  Also in the latter case, *left_deleted_ret
 * will be set.
 */
static AVL_INLINE struct avl_tree_node *
avl_handle_subtree_shrink(struct avl_tree_node ** const root_ptr,
			  struct avl_tree_node *parent,
			  const int sign,
			  bool * const left_deleted_ret)
{
	struct avl_tree_node *node;
	int old_balance_factor, new_balance_factor;

	old_balance_factor = avl_get_balance_factor(parent);

	if (old_balance_factor == 0) {
		/* Prior to the deletion, the subtree rooted at
		 * @parent was perfectly balanced.  It's now
		 * unbalanced by 1, but that's okay and its height
		 * hasn't changed.  Nothing more to do.  */
		avl_adjust_balance_factor(parent, sign);
		return NULL;
	}

	new_balance_factor = old_balance_factor + sign;

	if (new_balance_factor == 0) {
		/* The subtree rooted at @parent is now perfectly
		 * balanced, whereas before the deletion it was
		 * unbalanced by 1.  Its height must have decreased
		 * by 1.  No rotation is needed at this location,
		 * but continue up the tree.  */
		avl_adjust_balance_factor(parent, sign);
		node = parent;
	} else {
		/* @parent is too left-heavy (new_balance_factor == -2) or
		 * too right-heavy (new_balance_factor == +2).  */

		node = avl_get_child(parent, sign);

		/* The rotations below are similar to those done during
		 * insertion (see avl_handle_subtree_growth()), so full
		 * comments are not provided.  The only new case is the
		 * one where @node has a balance factor of 0, and that is
		 * commented.  */

		if (sign * avl_get_balance_factor(node) >= 0) {

			avl_rotate(root_ptr, parent, -sign);

			if (avl_get_balance_factor(node) == 0) {
				/*
				 * @node (B below) is perfectly balanced.
				 *
				 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				 * The comment, diagram, and equations
				 * below assume sign < 0.  The other case
				 * is symmetric!
				 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				 *
				 * Do a clockwise rotation rooted at
				 * @parent (A below):
				 *
				 *           A              B
				 *          / \           /   \
				 *         B   C?  =>    D     A
				 *        / \           / \   / \
				 *       D   E         F?  G?E   C?
				 *      / \
				 *     F?  G?
				 *
				 * Before the rotation:
				 *	balance(A) = -2
				 *	balance(B) =  0
				 * Let x = height(C).  Then:
				 *	height(B) = x + 2
				 *	height(D) = x + 1
				 *	height(E) = x + 1
				 *	max(height(F), height(G)) = x.
				 *
				 * After the rotation:
				 *	height(D) = max(height(F), height(G)) + 1
				 *		  = x + 1
				 *	height(A) = max(height(E), height(C)) + 1
				 *		  = max(x + 1, x) + 1 = x + 2
				 *	balance(A) = -1
				 *	balance(B) = +1
				 */

				/* A: -2 => -1 (sign < 0)
				 * or +2 => +1 (sign > 0)
				 * No change needed --- that's the same as
				 * old_balance_factor.  */

				/* B: 0 => +1 (sign < 0)
				 * or 0 => -1 (sign > 0)  */
				avl_adjust_balance_factor(node, -sign);

				/* Height is unchanged; nothing more to do.  */
				return NULL;
			} else {
				avl_adjust_balance_factor(parent, -sign);
				avl_adjust_balance_factor(node, -sign);
			}
		} else {
			node = avl_do_double_rotate(root_ptr, node,
						    parent, -sign);
		}
	}
	parent = avl_get_parent(node);
	if (parent)
		*left_deleted_ret = (node == parent->left);
	return parent;
}

/* Swaps node X, which must have 2 children, with its in-order successor, then
 * unlinks node X.  Returns the parent of X just before unlinking, without its
 * balance factor having been updated to account for the unlink.  */
static AVL_INLINE struct avl_tree_node *
avl_tree_swap_with_successor(struct avl_tree_node **root_ptr,
			     struct avl_tree_node *X,
			     bool *left_deleted_ret)
{
	struct avl_tree_node *Y, *ret;

	Y = X->right;
	if (!Y->left) {
		/*
		 *     P?           P?           P?
		 *     |            |            |
		 *     X            Y            Y
		 *    / \          / \          / \
		 *   A   Y    =>  A   X    =>  A   B?
		 *      / \          / \
		 *    (0)  B?      (0)  B?
		 *
		 * [ X unlinked, Y returned ]
		 */
		ret = Y;
		*left_deleted_ret = false;
	} else {
		struct avl_tree_node *Q;

		do {
			Q = Y;
			Y = Y->left;
		} while (Y->left);

		/*
		 *     P?           P?           P?
		 *     |            |            |
		 *     X            Y            Y
		 *    / \          / \          / \
		 *   A   ...  =>  A  ...   =>  A  ...
		 *       |            |            |
		 *       Q            Q            Q
		 *      /            /            /
		 *     Y            X            B?
		 *    / \          / \
		 *  (0)  B?      (0)  B?
		 *
		 *
		 * [ X unlinked, Q returned ]
		 */

		Q->left = Y->right;
		if (Q->left)
			avl_set_parent(Q->left, Q);
		Y->right = X->right;
		avl_set_parent(X->right, Y);
		ret = Q;
		*left_deleted_ret = true;
	}

	Y->left = X->left;
	avl_set_parent(X->left, Y);

	Y->parent_balance = X->parent_balance;
	avl_replace_child(root_ptr, avl_get_parent(X), X, Y);

	return ret;
}

/*
 * Removes an item from the specified AVL tree.
 *
 * @root_ptr
 *	Location of the AVL tree's root pointer.  Indirection is needed
 *	because the root node may change if the tree needed to be rebalanced
 *	because of the deletion or if @node was the root node.
 *
 * @node
 *	Pointer to the `struct avl_tree_node' embedded in the item to
 *	remove from the tree.
 *
 * Note: This function *only* removes the node and rebalances the tree.
 * It does not free any memory, nor does it do the equivalent of
 * avl_tree_node_set_unlinked().
 */
void
avl_tree_remove(struct avl_tree_node **root_ptr, struct avl_tree_node *node)
{
	struct avl_tree_node *parent;
	bool left_deleted = false;

	if (node->left && node->right) {
		/* @node is fully internal, with two children.  Swap it
		 * with its in-order successor (which must exist in the
		 * right subtree of @node and can have, at most, a right
		 * child), then unlink @node.  */
		parent = avl_tree_swap_with_successor(root_ptr, node,
						      &left_deleted);
		/* @parent is now the parent of what was @node's in-order
		 * successor.  It cannot be NULL, since @node itself was
		 * an ancestor of its in-order successor.
		 * @left_deleted has been set to %true if @node's
		 * in-order successor was the left child of @parent,
		 * otherwise %false.  */
	} else {
		struct avl_tree_node *child;

		/* @node is missing at least one child.  Unlink it.  Set
		 * @parent to @node's parent, and set @left_deleted to
		 * reflect which child of @parent @node was.  Or, if
		 * @node was the root node, simply update the root node
		 * and return.  */
		child = node->left ? node->left : node->right;
		parent = avl_get_parent(node);
		if (parent) {
			if (node == parent->left) {
				parent->left = child;
				left_deleted = true;
			} else {
				parent->right = child;
				left_deleted = false;
			}
			if (child)
				avl_set_parent(child, parent);
		} else {
			if (child)
				avl_set_parent(child, parent);
			*root_ptr = child;
			return;
		}
	}

	/* Rebalance the tree.  */
	do {
		if (left_deleted)
			parent = avl_handle_subtree_shrink(root_ptr, parent,
							   +1, &left_deleted);
		else
			parent = avl_handle_subtree_shrink(root_ptr, parent,
							   -1, &left_deleted);
	} while (parent);
}
/// END INLINE COPY OF AVL_TREE.C BY ERIC BIGGERS

/// Helper implementation

void FreePtr( void** ptrVar ) {
  if( ptrVar ) {
    if( *ptrVar ) {
      free( *ptrVar );
      *ptrVar = NULL;
    }
  }
}

/// String implementation

char* CreateString( const char* value ) {
  char* newString = NULL;
  size_t valueLen = 0;

  if( value ) { 
    valueLen = strlen(value);
  }

  newString = calloc(1, (valueLen + 1) * sizeof(char) );

  if( value && newString ) {
    strcat( newString, value );
  }

  return newString;
}

void FreeString( char** stringPtr ) {
  if( stringPtr ) {
    if( (*stringPtr) ) {
      free( (*stringPtr) );
      (*stringPtr) = NULL;
    }
  }
}

unsigned AppendString( char** destString, const char* value ) {
  if( destString == NULL ) { return 1; }
  if( value == NULL ) { return 2; }

  char* tmpString;

  size_t newLen = strlen(value);
  if( *destString == NULL ) {
    tmpString = calloc(1, (newLen + 1) * sizeof(char) );
    if( tmpString == NULL ) { return 3; }
  } else {
    newLen += strlen(*destString);
    tmpString = realloc(*destString, (newLen + 1) * sizeof(char));
    if( tmpString == NULL ) { return 4; }
  }

  strcat( tmpString, value );
  *destString = tmpString;

  return 0;
}

/// File Path implementation

unsigned SplitPath( const char* fromFullPath, char** toDir,
  char** toBaseName, char** toExt ) {

  const char* pathCh;
  const char* pathDir;
  const char* pathBaseName;
  const char* pathExt;
  size_t curLen = 0;
  size_t dirLen = 0;
  size_t basenameLen = 0;
  size_t extLen = 0;
  char* newDir = NULL;
  char* newBaseName = NULL;
  char* newExt = NULL;

  // Validate parameters
  // fromFullPath must be non-null and non-empty
  if( (fromFullPath == NULL) || (*fromFullPath == '\0') ) { return 1; }
  // toDir must be non-null, and point to a null pointer
  if( (toDir == NULL) || *toDir ) { return 2; }
  // toBaseName must be non-null, and point to a null pointer
  if( (toBaseName == NULL) || *toBaseName ) { return 3; }
  // toExt must be non-null, and point to a null pointer
  if( (toExt == NULL) || *toExt ) { return 4; }

  pathDir = fromFullPath;
  pathBaseName = fromFullPath;
  pathExt = fromFullPath;

  // Find last \ or / in file path
  pathCh = pathDir;
  curLen = 0;
  while( *pathCh )  {
    if( *pathCh == '\\' || *pathCh == '/' ) {
      pathBaseName = pathCh + 1;
      dirLen = curLen + 1;
    }
    pathCh++;
    curLen++;
  }

  // Find the first . after the last \ or /
  pathCh = pathBaseName;

  if( (*pathCh == '.') ) {
    // Special case .file as base name, when [dir\ or dir/].file[.ext]
    pathCh++;
    pathExt = pathCh;
    basenameLen++;
  }

  while( *pathCh )  {
    if( *pathCh == '.' ) {
      pathExt = pathCh;
      break;
    }
    pathCh++;
    basenameLen++;
  }

  // Get the length of the file extension
  pathCh = pathExt;
  extLen = 0;
  while( *pathCh )  {
    if( *pathCh == '.' ) {
      break;
    }
    pathCh++;
    pathExt++;
  }
  while( *pathCh ) {
    pathCh++;
    extLen++;
  }

  if( (dirLen | basenameLen | extLen) == 0 ) { return 5; }

  if( dirLen ) {
    newDir = malloc((dirLen + 1) * sizeof(char));
    if( newDir == NULL ) { goto ExitError; }
    memcpy( newDir, pathDir, dirLen * sizeof(char) );
    newDir[dirLen] = '\0';
    *toDir = newDir;
  }

  if( basenameLen ) {
    newBaseName = malloc((basenameLen + 1) * sizeof(char));
    if( newBaseName == NULL ) { goto ExitError; }
    memcpy( newBaseName, pathBaseName, basenameLen * sizeof(char) );
    newBaseName[basenameLen] = '\0';
    *toBaseName = newBaseName;
  }

  if( extLen ) {
    newExt = malloc((extLen + 1) * sizeof(char));
    if( newExt == NULL ) { goto ExitError; }
    memcpy( newExt, pathExt, extLen * sizeof(char) );
    newExt[extLen] = '\0';
    *toExt = newExt;
  }

  return 0;

ExitError:
  FreePtr( &newDir );
  FreePtr( &newBaseName );
  FreePtr( &newExt );
  return 6;
}

unsigned JoinPath( const char* fromDir, const char* fromBaseName,
  const char* fromExt, char** toFullPath ) {

  char* newPath = NULL;
  size_t totalLen;
  const char* tmpCh;
  char pathSeparator = '\0';
  char extDot = '\0';

  // Validate parameters
  if( (toFullPath == NULL) || *toFullPath ) { return 4; }
  if( (fromDir == NULL) && (fromBaseName == NULL)
    && (fromExt == NULL) ) { return 5; }

  // Calculate the total string length
  totalLen = 0;
  if( fromDir ) {
    totalLen = strlen(fromDir);
    if( totalLen ) {
      // Check for trailing path separator
      if( (fromDir[totalLen - 1] != '\\') &&
          (fromDir[totalLen - 1] != '/') ) {
        totalLen++;
        // If path separator isn't at end, detect which slash to use
        tmpCh = fromDir;
        pathSeparator = '\\'; // default to '\'
        while( *tmpCh ) {
          if( (*tmpCh == '\\') || (*tmpCh == '/') ) {
            pathSeparator = *tmpCh;
            break;
          }
          tmpCh++;
        }
      }
    }
  }

  if( fromBaseName ) {
    totalLen += strlen(fromBaseName);
    // Skip slash at start of BaseName
    if( (*fromBaseName == '\\') || (*fromBaseName == '/') ) {
      fromBaseName++;
      totalLen--;
    }
    // If a slash remains, the base name is [probably?] invalid
    if( (*fromBaseName == '\\') || (*fromBaseName == '/') ) { return 6; }
  }

  if( fromExt ) {
    totalLen += strlen(fromExt);
    // Add a dot if missing from file extension
    if( *fromExt != '.' ) {
      extDot = '.';
      totalLen++;
    }
  }

  // Allocate string buffer
  newPath = malloc((totalLen + 1) * sizeof(char));
  if( newPath == NULL ) { return 7; }
  *newPath = '\0';

  // Copy in each parameter
  if( fromDir ) {
    strcat( newPath, fromDir );
    if( pathSeparator ) {
      strncat( newPath, &pathSeparator, 1 );
    }
  }

  if( fromBaseName ) {
    strcat( newPath, fromBaseName );
  }

  if( fromExt ) {
    if( extDot ) {
      strncat( newPath, &extDot, 1 );
    }
    strcat( newPath, fromExt );
  }

  *toFullPath = newPath;
  return 0;
}

/// Symbol Table implementation

SymbolTable* CreateSymbolTable() {
  SymbolTable* newSymbolTable = NULL;

  return NULL;
}

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
}

unsigned DeclareNamespace( SymbolTable* symbolTable,
  const char* namespace ) {

  if( symbolTable == NULL ) { return 1; }
  if( namespace == NULL ) { return 2; }

  return 0;
}

/// Parser implementation

Parser* OpenGrammar( const char* fileName ) {
  Parser* newParser = NULL;

  if( !(fileName && (*fileName)) ) { return NULL; }

  newParser = calloc(1, sizeof(Parser));
  if( newParser == NULL ) { return NULL; }

  newParser->handle = fopen(fileName, "r");
  if( newParser->handle ) {
    ReadChar( newParser );

    newParser->line = 1;
    newParser->column = 1;
    newParser->nextLine = 1;
    newParser->nextColumn = 1;
 
    ReadChar( newParser );

    return newParser;
  }

  ReleaseGrammar( &newParser );
  return NULL;
}

void CloseGrammar( Parser* grammar ) {
  if( grammar && grammar->handle ) {
    fclose( grammar->handle );
    grammar->handle = NULL;
  }
}

void ReleaseGrammar( Parser** grammarPtr ) {
  if( grammarPtr ) {
    if( (*grammarPtr) ) {
      CloseGrammar( (*grammarPtr) );

      free( (*grammarPtr) );
      (*grammarPtr) = NULL;
    }
  }
}

int ReadChar( Parser* grammar ) {
  if( !(grammar && grammar->handle) ) { return EOF; }

  grammar->ch = grammar->nextch;
  grammar->line = grammar->nextLine;
  grammar->column = grammar->nextColumn;

  grammar->nextch = fgetc(grammar->handle);
  if( grammar->nextch != EOF ) {
    if( grammar->nextch == '\r' ) {
      grammar->nextch = fgetc(grammar->handle);
      if( grammar->nextch != '\n' ) {
        ungetc( grammar->nextch, grammar->handle );
      }
      grammar->nextch = '\n';
    }

    if( grammar->ch == '\n' ) {
      grammar->nextLine++;
      grammar->nextColumn = 0;
    }
    grammar->nextColumn++;

    return grammar->ch;
  }

  return EOF;
}

unsigned SkipSpace( Parser* grammar ) {
  if( !(grammar && grammar->handle) ) { return 1; }

  if( isspace(grammar->ch) ) {
    while( isspace(grammar->ch) ) {
      ReadChar( grammar );
    }
    return SKIPPED;
  }

  return NOTHINGTOSKIP;
}

unsigned SkipCommentsAndSpace( Parser* grammar ) {
  unsigned result;
  unsigned commentLevel = 0;

  if( !(grammar && grammar->handle) ) { return 1; }

  while( grammar->ch != EOF ) {
    result = 0;

    if( isspace(grammar->ch) ) {
      while( isspace(grammar->ch) ) {
        ReadChar( grammar );
      }

      result |= 1;
    }

    if( (grammar->ch == '/') && (grammar->nextch == '/') ) {
      ReadChar( grammar );
      ReadChar( grammar );

      while( grammar->ch != EOF ) {
        if( grammar->ch == '\n' ) {
          break;
        }

        ReadChar( grammar );
      }

      result |= 2;
    }

    if( (grammar->ch == '/') && (grammar->nextch == '*') ) {
      ReadChar( grammar );
      ReadChar( grammar );

      commentLevel = 1;

      while( commentLevel && (grammar->ch != EOF) ) {
        if( (grammar->ch == '/') && (grammar->nextch == '*') ) {
          if( commentLevel == ((unsigned)-1) ) {
            printf( "Error[L%u, C%u]: Maximum nested comment level reached.\n",
              grammar->line, grammar->column );
            exit(3);
          }
          commentLevel++;

          ReadChar( grammar );
          ReadChar( grammar );
          continue;
        }

        if( (grammar->ch == '*') && (grammar->nextch == '/') ) {
          if( commentLevel == 0 ) {
            printf( "Error[L%u, C%u]: Comment level mismatch.\n",
              grammar->line, grammar->column );
            exit(3);
          }
          commentLevel--;

          ReadChar( grammar );
          ReadChar( grammar );
          continue;
        }

        ReadChar( grammar );
      }

      result |= 4;
    }

    if( result == 0 ) {
      break;
    }
  }

  return SKIPPED;
}

unsigned MarkParserPosition( Parser* grammar ) {
  if( !(grammar && grammar->handle) ) { return 1; }

  return 2;
}

unsigned RestoreParserPosition( Parser* grammar ) {
  if( !(grammar && grammar->handle) ) { return 1; }

  return 2;
}

unsigned ReadIdent( Parser* grammar, char** identPtr ) {
  if( grammar == NULL ) { return 1; }
  if( !(identPtr && (*identPtr == NULL)) ) { return 2; }

  char* newIdent = NULL;
  char identBuffer[32] = {};
  unsigned ibIndex = 0;
  unsigned result = 0;

  if( (grammar->ch == '_') || isalpha(grammar->ch) ) {
    while( (grammar->ch == '_') || isalnum(grammar->ch) ) {
      if( ibIndex == 31 ) {
        result = AppendString(&newIdent, identBuffer);
        if( result ) {
          printf( "Internal error: ReadIdent[%u]\n", result );
          exit(4);
        }
        identBuffer[0] = '\0';
        ibIndex = 0;
      }

      identBuffer[ibIndex] = grammar->ch;
      identBuffer[ibIndex + 1] = '\0';
      ibIndex++;

      ReadChar( grammar );
    }
    if( ibIndex ) {
      result = AppendString(&newIdent, identBuffer);
      if( result ) {
        printf( "Internal error: ReadIdent[%u]\n", result );
        exit(4);
      }
    }

    *identPtr = newIdent;
    return 0;
  }

  FreeString( &newIdent );

  return 3;
}

unsigned ParseGrammar( Parser* grammar, SymbolTable* symbolTable,
  Compiler* compiler ) {

  unsigned result;

  if( !(grammar && grammar->handle) ) { return 1; }
//  if( symbolTable == NULL ) { return 2; }
  if( !(compiler && compiler->sourceHandle &&
    compiler->importHandle) ) { return 3; }

  SkipCommentsAndSpace( grammar );
  result = ReadIdent(grammar, &ident);
  if( result ) {
    printf( "Error reading identifier: %u\n", result );
    exit(3);
  }

//  return 4;
  return 0;
}

/// Compiler implementation

Compiler* CreateCompiler( const char* sourceFileName,
  const char* importFileName ) {

  if( !(sourceFileName && (*sourceFileName)) ) { return NULL; };

  Compiler* newCompiler = NULL;

  newCompiler = calloc(1, sizeof(Compiler));
  if( newCompiler == NULL ) { return NULL; }

  newCompiler->sourceHandle = fopen(sourceFileName, "w");
  newCompiler->importHandle = fopen(importFileName, "w");

  if( newCompiler->sourceHandle && newCompiler->importHandle ) {
    fprintf( newCompiler->sourceHandle,
      "\n"
      "// Generated by Origo\n\n"
      "#include <stdio.h>\n\n"
      "/// DECLARATIONS\n\n"
    );

    newCompiler->outCode = CreateString(
      "/// IMPLEMENTATION\n\n"
      "int main( int argc, char** argv ) {\n"
      "  return 0;\n"
      "}\n"
    );
    return newCompiler;
  }

  ReleaseCompiler( &newCompiler );
}

void CloseCompiler( Compiler* compiler ) {
  if( compiler ) {
    // Write the generated code following previously written declarations
    if( compiler->sourceHandle && compiler->outCode ) {
      fprintf( compiler->sourceHandle, "%s", compiler->outCode );

      // Release the compiler implementation string
      free( compiler->outCode );
      compiler->outCode = NULL;
    }

    // Close source file
    if( compiler->sourceHandle ) {
      fclose( compiler->sourceHandle );
      compiler->sourceHandle = NULL;
    }

    // Close import file
    if( compiler->importHandle ) {
      fclose( compiler->importHandle );
      compiler->importHandle = NULL;
    }
  }
}

void ReleaseCompiler( Compiler** compilerPtr ) {
  if( compilerPtr ) {
    if( (*compilerPtr) ) {
      CloseCompiler( (*compilerPtr) );

      free( *compilerPtr );
      (*compilerPtr) = NULL;
    }
  }
}

int BuildCompiler( const char* sourceFileName,
  const char* exeFileName, const char* importFileName ) {

  if( !(sourceFileName && (*sourceFileName)) ) { return 0; };
  if( !(exeFileName && (*exeFileName)) ) { return 0; };
  if( !(importFileName && (*importFileName)) ) { return 0; };

  char* tccOptions[] = {
    " ",
    (char*)sourceFileName,
    " ",
    (char*)importFileName,
    "-o",
    (char*)exeFileName,
    NULL
  };
  return _spawnvp(_P_WAIT, ".\\tools\\tcc\\tcc.exe", tccOptions);
}

int PatchVersion( const char* fileName,
  const char* version, const char* versionstr,
  const char* description, const char* copyright ) {

  char* verpatchOptions[] = {
    /* [0] */ " ",
    /* [1] */ (char*)fileName,
    /* [2] */ "/va ",
    /* [3] */ "0.0.0.0", // version
    /* [4] */ " /pv ",
    /* [5] */ "0.0.0.0", // versionstr
    /* [6] */ " /s desc \"",
    /* [7] */ "", // description
    /* [8] */ "\" /s copyright \"",
    /* [9] */ "", // copyright
    /* [10] */ "\"",
    NULL
  };

  if( copyright ) {
    verpatchOptions[9] = (char*)copyright;
  }

  if( description ) {
    verpatchOptions[7] = (char*)description;
  }

  if( version ) {
    verpatchOptions[3] = (char*)version;
  }

  if( versionstr ) {
    verpatchOptions[5] = (char*)versionstr;
  }

  return _spawnvp(_P_WAIT,
    ".\\tools\\verpatch\\verpatch.exe", verpatchOptions);
}
