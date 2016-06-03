<?php 
/**
 * 分页类
 * @author fanhaobai & fanhaobai@gmail.com
 * @date 2016-05-21
 * 
 * eg:	$page = Page::getInstance($records,$pagesize,$config)
 * 必选参数：$records 记录总数
 * 					$pagesize 每页展示的记录数
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
private $pagesize;        //每页的记录数【必须】
//配置项,可缺省(数组形式)
private $pagelistsize;    //翻页列表的大小【不建议修改,需大于或等于5】
private $pagename;        //传参时页码的名称,默认：page
private $theme;           //主题样式：simple--简单样式 default--默认样式
private $toolbars;        //工具使能【数组】：up--上一页 down--下一页 jump--跳转到第n页
private $align;           //位置：left--居左 center--居中 right--居右 
//缓存
private $page;            //当前页
private $pages;	          //总页数
private $startrow;        //记录的起始行号,可通过getStartRow()获取
private $startpage;       //滑动列表起始页
private $endpage;         //滑动列表结束页
private $uri = NULL;
private static $instance; //实例,可通过getInstance()创建或获取
/**
 * 私有克隆
 */
private function __clone(){}
/**
 * 私有构造方法
 * @param $records 总记录数
 * @param $pagesize 每页记录数
 * @param $config 配置数组【可缺省】
 */
private function __construct($records, $pagesize, array $config = NULL)
{
    //必选参数
    $this->records = $records;
    $this->pagesize = $pagesize;
    //可选配置
    //按钮工具初始化
    $this->theme = isset($config['theme']) ? $config['theme'] : 'default';
    $this->toolbars = isset($config['toolbars']) ? $config['toolbars'] : array('up','down','jump');
    $this->pagename = isset($config['pagename']) ? $config['pagename'] : 'page';
    //列表对齐方式,容错处理
    if (isset($config['align']) && in_array($config['align'], array('left','center','right')))
        $this->align = $config['align'];
    else
        $this->align = 'center';
    //分页列表大小初始化
    if (isset($config['pagelistsize']) && $config['pagelistsize']>=5 && $config['pagelistsize']<=15)
        $this->pagelistsize = $config['pagelistsize'];
    else {
        if($this->theme == 'simple')    //简单样式时pagelistsize为10
            $this->pagelistsize = 10;
        else
            $this->pagelistsize = 7;
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
    $this->pages = (int)ceil($this->records / $this->pagesize);
}
/**
 * 获取当前页页码
 */
private function setPage()
{
    if (!isset($_GET[$this->pagename]) || $_GET[$this->pagename]<1 || $_GET[$this->pagename]>$this->pages) {
        $this->page = 1;
        $this->startrow = 0;
    } else {
        $this->page = (int)$_GET[$this->pagename];
        $this->startrow = ($this->page-1) * $this->pagesize;
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
        unset($params[$this->pagename]);
        $uri = $parse['path'] . "?" . http_build_query($params);
    }
    if (!empty($params)) {      //各参数之间用‘&’分割,如果只是$this->pagename参数,则不需要分割
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
    return $this->uri . $this->pagename . '=' . $page;
}
/**
 * 获取滑动列表的起始页和结束页
 * @param $minpage 滑动翻页列表的最小页页码
 * @param $pagelists 滑动翻页列表的大小
 */
private function glideList($minpage, $pagelists)
{
    //当前页在翻页列表居中固定时,距离滑动起始页和结束页的页数
    $left = (int)floor($pagelists/2);
    $right = $pagelists-$left-1;
    //滑动起始页和结束页位置
    $this->startpage = $this->page-$left;       //起始页
    $this->endpage = $this->page+$right;        //结束页
    //总页数小于翻页列表的总页数,翻页列表不进行滑动
    if ($this->pages < ($pagelists + $minpage - 1)) {
        $this->startpage = $minpage;              //起始页为1
        $this->endpage = $this->pages;            //结束页为pages
    } else if ($this->startpage < $minpage) {     //滑动起始页小于滑动列表的最小页数,翻页列表不进行滑动
        $this->startpage = $minpage;
        $this->endpage = $this->startpage + $pagelists - 1;
    } else if ($this->endpage > $this->pages) {   //滑动结束页页大于滑动列表的最大页数(即总页数),翻页列表不进行滑动
        $this->endpage = $this->pages;
        $this->startpage = $this->endpage - $pagelists + 1;
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
        $jump = "javascript:var i=document.getElementById('jump').value;this.href='" . $this->uri . $this->pagename . "='" . "+i;";
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
        $this->glideList(3, $this->pagelistsize - 2);   //列表滑动的起始页从第3页开始
        if ($this->startpage > 3)
            $str .= '<span>...</span>';                //省略页码
    } else {   //simple样式/dafault样式时当前页小于3,翻页列表所有页码都产生滑动效果
        $this->glideList(1, $this->pagelistsize);
    }
    //滑动翻页列表进行滑动
    for ($i=$this->startpage;$i<=$this->endpage;$i++) {
        if ($i == $this->page)                         //当前页无连接
            $str .= '<strong>' . $i . '</strong>';
        else
            $str .= '<a class="page" href="' . $this->setUri($i) . '">' . $i . '</a>' . "\r\n";
    }
    //省略页码
    if ($this->theme == 'default') {
        if ($this->pages > $this->pagelistsize && $this->endpage != $this->pages)
            $str .= '<span>...</span>';
    }
    return $str;
}
/**
 * 查询每页记录的起始行号 
 * @return int
 */
public function getStartRow()
{
    return $this->startrow;
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
/**
 * 得到分页对象
 * @param $records
 * @param $pagesize
 * @param $config 可选配置参数
 * @return obj
 */
public static function getInstance($records, $pagesize, array $config = NULL)
{
    if (!(self::$instance instanceof self)) {
        self::$instance = new Page($records, $pagesize, $config);
    }
    return self::$instance;
}
/*---end---*/
}
?>