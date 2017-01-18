package com.centerm.dispatch.app;

/**
 * 应用程序控制统一接口
 *
 */
public interface IAppCtrl 
{

	/**
	 * 创建应用程序进程，让其运行在后台
	 * @param action 对应的动作行为
	 * @return true表示创建成功，false表示创建失败
	 */
	boolean create( Action action );
	
	/**
	 * 显示应用程序
	 * @param action 对应的动作行为
	 * @return true表示创建成功，false表示创建失败
	 */
	boolean start( Action action );
	
	
	/**
	 * 关闭应用程序界面
	 * @param action 对应的动作行为
	 * @return true表示操作成功，false表示操作失败
	 */
	boolean close( Action action );
	
	/**
	 * 处理应用程序数据
	 * @param action 对应的动作行为
	 * @return true表示操作成功，false表示操作失败
	 */
	boolean dealdata( Action action );
	
	//TODO:未来还要增加其它接口
}
