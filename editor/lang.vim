" Syntax Highlighting
" Language: .lang
"
" Usage: 
"   - Vim:
"       ~/.vim/syntax/lang.vim
"   - Neovim
"       $VIMRUNTIME/syntax/lang.vim
"   - Add to .vimrc
"       autocmd BufRead,BufNewFile *.lang set filetype=lang

if exists("b:current_sytnax")
    finish
endif

" Functions (calls and definitions)
syntax match langFuncIds '\w\+\s*\ze(' 

" Keywords
syntax keyword langFuncKwd func nextgroup=langFuncIds skipwhite
syntax keyword langKwds return else if 

" Types
syntax keyword langTypes int void

" Numeric literals
syntax match langNums '\d\+\.\?\d*'

" Comments
syntax keyword langTodos TODO FIXME XXX NOTE
syntax match langComments '//.*$' contains=langTodos



" Highlight groups
highlight default link langFuncKwd Keyword
highlight default link langFuncIds Function 
highlight default link langFuncCalls Function
highlight default link langKwds Keyword
highlight default link langTypes Type
highlight default link langNums Number
highlight default link langTodos Todo
highlight default link langComments Comment

let b:current_syntax = "lang"
