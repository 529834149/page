<?php 
/**
 * 分页类
 * @author fanhaobai & fanhaobai@gmail.com
 * @date 2016-05-21
 *
 * eg:	$page = new Page($records,$pagesize,$config)
 * 必选参数：$records 记录总数
 *         $pagesize 每页展示的记录数
 * 可选配置项：$config 配置数组
 * 								
 * simple:   1 2 3 4 5 6 7 8 9 10
 * default:  1 2 ... 5 6 7 8 9 ...
 * 
 * 分页样式文件:Page.css,link到需要分页的页面中就OK，修改CSS样式文件可以轻松的得到不同的分页样式
*/	
//设置自动加载
spl_autoload_register("autoLoad");
function autoLoad($classname){
    $filename="./$classname.class.php";
    if(file_exists($filename))
        include_once($filename);
}
//基本参数
$records = 70;  //记录总数
$pageSize = 5;  //每页显示的记录数【当所有记录只能够显示1页时,不会出现分页列表】
//配置数组【可缺省,表示默认配置】
$config = array(
    'pagelistsize'  => 7         //翻页列表的大小【不能小于5,默认simple--10 default--7】
    ,'pagename'     => 'page'   //get方式传递和解析翻页参数时的参数名称【默认page】
    ,'theme'        => 'default'
              /*主题【默认default】： simple  上一页 1 2 3 4 5 6 7 8 9 10 下一页
                                    default 上一页 1 2 ... 5 6 7 8 9 ... 下一页 总页数 跳转到n页 */
    ,'toolbars'     => array('up','down','jump')
              /*工具栏使能【数组形式,需要开启谁就向数组中加入对应值,默认全部使能,simple下使能jump无效】:
                             up--上一页 down--下一页 jump--跳转到n页 */
    ,'align'        => 'center'
             /*列表对齐方式【相对于其父元素,默认center】: left--左对齐 center--居中 right--右对齐 */
);
//获取单例分页对象
$page = new Page($records, $pageSize, $config);
//获取记录的起始行
$startRow = $page->getStartRow();
$currentPage = $page->getCurrentPage();
echo "记录起始行为：" . $startRow . "；每页显示记录数为：" . $pageSize;
echo "<hr>";
echo "当前页：" . $currentPage;
?>
<!DOCTYPE html>
<html lang="zh">
<head>
<meta charset="UTF-8">
<title>分页演示</title>
<!--链接page.css-->
<link rel="stylesheet" type="text/css" href="./Page.css">
</head>
<body>
<div style="width:90%;border:1px solid red;">
    <!--展示分页列表,列表的宽度继承其父元素的宽度-->
    <?php echo $page->pagination() ?>
</div>
</body>
</html>