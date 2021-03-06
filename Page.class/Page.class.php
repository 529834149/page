<?php 
/**
 * 分页类
 * @author fanhaobai & fanhaobai@gmail.com
 * @date 2016-05-21
 * 
 * eg:	$page =new Page($records,$pageSize,$config)
 * 必选参数：$records 记录总数
 *         $pageSize 每页展示的记录数
 * 可选配置项：$config 配置数组
 * 								
 * simple:   1 2 3 4 5 6 7 8 9 10
 * default:  1 2 ... 5 6 7 8 9 ...
 * 
 * 分页样式文件:Page.css,link到需要分页的页面中就OK. 
*/
final class Page
{
    //参数
    private $records;         //总记录数【必须】
    private $pageSize;        //每页的记录数【必须】
    //配置项,可缺省(数组形式)
    private $pageListSize;    //翻页列表的大小【不建议修改,需大于或等于5】
    private $pageName;        //传参时页码的名称,默认：page
    private $theme;           //主题样式：simple--简单样式 default--默认样式
    private $toolbars;        //工具使能【数组】：up--上一页 down--下一页 jump--跳转到第n页
    private $align;           //位置：left--居左 center--居中 right--居右
    //缓存
    private $page;            //当前页
    private $pages;	          //总页数
    private $startRow;        //记录的起始行号,可通过getstartRow()获取
    private $startPage;       //滑动列表起始页
    private $endPage;         //滑动列表结束页
    private $uri = NULL;

/**
 * 私有构造方法
 * @param $records 总记录数
 * @param $pageSize 每页记录数
 * @param $config 配置数组【可缺省】
 */
public function __construct($records, $pageSize, array $config = NULL)
{
    //必选参数
    $this->records = $records;
    $this->pageSize = $pageSize;
    //可选配置
    //按钮工具初始化
    $this->theme = isset($config['theme']) ? $config['theme'] : 'default';
    $this->toolbars = isset($config['toolbars']) ? $config['toolbars'] : array('up','down','jump');
    $this->pageName = isset($config['pageName']) ? $config['pageName'] : 'page';
    //列表对齐方式,容错处理
    if (isset($config['align']) && in_array($config['align'], array('left','center','right')))
        $this->align = $config['align'];
    else
        $this->align = 'center';
    //分页列表大小初始化
    if (isset($config['pageListSize']) && $config['pageListSize'] >= 5 && $config['pageListSize'] <= 15)
        $this->pageListSize = $config['pageListSize'];
    else {
        if($this->theme == 'simple')    //简单样式时pageListSize为10
            $this->pageListSize = 10;
        else
            $this->pageListSize = 7;
    }
    //其他属性初始化
    $this->setPages();          //初始化总页数
    $this->setPage();           //获得当前页和记录的起始行号
}
/**
 * 获取总页数
 */
private function setPages()
{
    $this->pages = (int)ceil($this->records / $this->pageSize);
}
/**
 * 获取当前页页码
 */
private function setPage()
{
    if (!isset($_GET[$this->pageName]) || $_GET[$this->pageName] < 1 || $_GET[$this->pageName] > $this->pages) {
        $this->page = 1;
        $this->startRow = 0;
    } else {
        $this->page = (int)$_GET[$this->pageName];
        $this->startRow = ($this->page - 1) * $this->pageSize;
    }
}
/**
 * 获取不包含$this->page参数的URI
 * @return string
 */
private function getUri()
{
    //uri后必须追加上‘?’,方便后续处理,这里得到的uri不包含page
    $uri = $_SERVER['REQUEST_URI'] . (strpos($_SERVER['REQUEST_URI'], '?') ? '' : '?');
    $parse = parse_url($uri);           //解析uri
    if (isset($parse['query'])) {       //get传值参数
        parse_str($parse['query'], $params);
        unset($params[$this->pageName]);
        $uri = $parse['path'] . "?" . http_build_query($params);
    }
    if (!empty($params)) {      //各参数之间用‘&’分割,如果只是$this->pageName参数,则不需要分割
        $uri .= '&';
    }
    return $this->uri = $uri;
}
/**
 * 设定链接到的URI
 * @param $page 设定需要链接到的页码
 * @return string
 */
private function setUri($page)
{
    if ($this->uri === NULL)
        $this->getUri();
    return $this->uri . $this->pageName . '=' . $page;
}
/**
 * 获取滑动列表的起始页和结束页
 * @param $minpage 滑动翻页列表的最小页页码
 * @param $pagelists 滑动翻页列表的大小
 */
private function glideList($minpage, $pagelists)
{
    //当前页在翻页列表居中固定时,距离滑动起始页和结束页的页数
    $left = (int)floor($pagelists / 2);
    $right = $pagelists - $left - 1;
    //滑动起始页和结束页位置
    $this->startPage = $this->page - $left;       //起始页
    $this->endPage = $this->page + $right;        //结束页
    //总页数小于翻页列表的总页数,翻页列表不进行滑动
    if ($this->pages < ($pagelists + $minpage - 1)) {
        $this->startPage = $minpage;              //起始页为1
        $this->endPage = $this->pages;            //结束页为pages
    } else if ($this->startPage < $minpage) {     //滑动起始页小于滑动列表的最小页数,翻页列表不进行滑动
        $this->startPage = $minpage;
        $this->endPage = $this->startPage + $pagelists - 1;
    } else if ($this->endPage > $this->pages) {   //滑动结束页页大于滑动列表的最大页数(即总页数),翻页列表不进行滑动
        $this->endPage = $this->pages;
        $this->startPage = $this->endPage - $pagelists + 1;
    }
}
/**
 * 上一页
 * @return string
 */
private function pageUp()
{
    if (in_array('up', $this->toolbars)) {
        if ($this->page == 1) {                 //当前页为1,禁止上一页按钮
            $style = 'button disable';
            $href = "";
        } else {
            $style = 'button enable';           //使能上一页按钮
            $href = 'href="' . $this->setUri($this->page - 1) . '"';
        }
        return '<a class="' . $style . '"' . $href . '><b><</b>上一页</a>' . "\r\n";
    }
}
/**
 * 下一页
 * @return string
 */
private function pageDown()
{
    if (in_array('down', $this->toolbars)) {
        if ($this->page == $this->pages) {       //当前页为最后一页,禁止下一页按钮
            $style = 'button disable';
            $href = "";
        } else {
            $style = "button enable";            //使能下一页按钮
            $href = 'href="' . $this->setUri($this->page + 1) . '"';
        }
        return '<a class="' . $style . '"' . $href . '>下一页<b>></b></a>' . "\r\n";
    }
}
/**
 * 跳转工具
 * @return string
 */
private function pageJump()
{
    $str = '';
    if ($this->theme == 'default' && in_array('jump', $this->toolbars)) {
        $str .= '&nbsp;共' . $this->pages . '页&nbsp;&nbsp;到第';
        $str .= '<input id="jump" type="text" value="' . $this->page . '"></input>' . "\r\n";
        $str .= '页';
        $jump = "javascript:var i=document.getElementById('jump').value;this.href='" . $this->uri . $this->pageName . "='" . "+i;";
        $str .= '<a class="page go" href="" onclick="' . $jump . '">确定</a>' . "\r\n";
    }
    return $str;
}
/**
 * 获得翻页列表
 * @return string
 */
private function pageList()
{
    $str = '';
    //default样式,翻页列表的第1、2页不产生滑动效果,方便用户快速跳转到第1、2页
    if ($this->theme == 'default' && $this->page >= 3) {
        //当前页大于或等于3,翻页列表的第1、2页不产生滑动效果
        $str = '<a class="page" href="' . $this->setUri(1) . '">1</a>' . "\r\n";
        $str .= '<a class="page" href="' . $this->setUri(2) . '">2</a>' . "\r\n";
        $this->glideList(3, $this->pageListSize - 2);   //列表滑动的起始页从第3页开始
        if ($this->startPage > 3)
            $str .= '<span>...</span>';                //省略页码
    } else {   //simple样式/dafault样式时当前页小于3,翻页列表所有页码都产生滑动效果
        $this->glideList(1, $this->pageListSize);
    }
    //滑动翻页列表进行滑动
    for ($i=$this->startPage; $i<=$this->endPage; $i++) {
        if ($i == $this->page)                         //当前页无连接
            $str .= '<strong>' . $i . '</strong>';
        else
            $str .= '<a class="page" href="' . $this->setUri($i) . '">' . $i . '</a>' . "\r\n";
    }
    //省略页码
    if ($this->theme == 'default') {
        if ($this->pages > $this->pageListSize && $this->endPage != $this->pages)
            $str .= '<span>...</span>';
    }
    return $str;
}
/**
 * 查询每页记录的起始行号 
 * @return int
 */
public function getstartRow()
{
    return $this->startRow;
}
/**
 * 查询当前页码
 * @return int
 */
public function getCurrentPage()
{
    return $this->page;
}
/**
 * 展示分页列表
 * @return string 
 */
public function pagination()
{
    $str = '';
    //分页列表宽度从其父元素继承
    if($this->pages > 1){         //总页数大于1页才分页
        $str = '<div class="pagination" style="text-align:' . $this->align . '">' . "\r\n";
        $str .= $this->pageUp();    //上一页
        $str .= $this->pageList();  //翻页列表
        $str .= $this->pageDown();  //下一页
        $str .= $this->pageJump();  //跳转
        $str .= '</div>' . "\r\n";
    }
    return $str;
}
/*---end---*/
}
?>