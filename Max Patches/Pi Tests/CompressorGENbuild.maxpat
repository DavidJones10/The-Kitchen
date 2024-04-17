{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 6,
			"revision" : 1,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 59.0, 106.0, 640.0, 480.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"autosave" : 1,
					"id" : "obj-1",
					"inletInfo" : 					{
						"IOInfo" : [  ]
					}
,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outletInfo" : 					{
						"IOInfo" : [  ]
					}
,
					"outlettype" : [ "list" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 8,
							"minor" : 6,
							"revision" : 1,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "rnbo",
						"rect" : [ 59.0, 606.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Lato",
						"gridonopen" : 1,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"assistshowspatchername" : 0,
						"title" : "untitled",
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-3",
									"linecount" : 7,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 268.0, 111.0, 150.0, 107.0 ],
									"text" : "Params:\n\ninputLevel\nthreshold\nratio\nattack\nrelease"
								}

							}
, 							{
								"box" : 								{
									"genpatcher" : 									{
										"patcher" : 										{
											"fileversion" : 1,
											"appversion" : 											{
												"major" : 8,
												"minor" : 6,
												"revision" : 1,
												"architecture" : "x64",
												"modernui" : 1
											}
,
											"classnamespace" : "dsp.gen",
											"rect" : [ 829.0, 339.0, 600.0, 450.0 ],
											"bglocked" : 0,
											"openinpresentation" : 0,
											"default_fontsize" : 12.0,
											"default_fontface" : 0,
											"default_fontname" : "Arial",
											"gridonopen" : 1,
											"gridsize" : [ 15.0, 15.0 ],
											"gridsnaponopen" : 1,
											"objectsnaponopen" : 1,
											"statusbarvisible" : 2,
											"toolbarvisible" : 1,
											"lefttoolbarpinned" : 0,
											"toptoolbarpinned" : 0,
											"righttoolbarpinned" : 0,
											"bottomtoolbarpinned" : 0,
											"toolbars_unpinned_last_save" : 0,
											"tallnewobj" : 0,
											"boxanimatetime" : 200,
											"enablehscroll" : 1,
											"enablevscroll" : 1,
											"devicewidth" : 0.0,
											"description" : "",
											"digest" : "",
											"tags" : "",
											"style" : "",
											"subpatcher_template" : "",
											"assistshowspatchername" : 0,
											"boxes" : [ 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 619.0, 365.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-35"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 408.0, 377.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-34"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "* 0.001",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 522.0, 295.0, 47.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-33"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "samplerate",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 522.0, 271.0, 68.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-32"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "param release @min 5 @max 2000",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 619.0, 341.0, 202.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-31"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "param attack @min 0.5 @max 2000",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 408.0, 341.0, 201.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-30"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "* -1",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 329.0, 294.0, 27.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-29"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "slide",
														"numinlets" : 3,
														"numoutlets" : 1,
														"patching_rect" : [ 131.0, 327.0, 40.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-27"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "rsub 1",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 437.0, 177.0, 42.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-25"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "rsub 1",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 329.0, 266.0, 42.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-24"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "dbtoa",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 329.0, 237.0, 39.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-23"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 329.0, 213.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-22"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "rdiv 1",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 437.0, 151.0, 38.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-21"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "-",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 329.0, 156.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-20"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "param ratio @min 1 @max 20",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 437.0, 122.0, 169.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-19"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "+ 1",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 131.0, 287.0, 26.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-18"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 131.0, 257.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-17"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 228.0, 389.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-16"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 50.0, 387.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-15"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "param threshold @min -70 @max 6",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 498.0, 14.0, 199.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-14"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "> threshold",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 131.0, 213.0, 68.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-13"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "atodb",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 131.0, 184.0, 39.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-12"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "abs",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 131.0, 156.0, 28.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-11"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "param inputLevel @min -70 @max 15",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 278.0, 14.0, 211.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-9"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "dbtoa",
														"numinlets" : 1,
														"numoutlets" : 1,
														"patching_rect" : [ 278.0, 40.0, 39.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-8"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 231.0, 95.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-7"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "*",
														"numinlets" : 2,
														"numoutlets" : 1,
														"patching_rect" : [ 50.0, 95.0, 29.5, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-6"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "out 2",
														"numinlets" : 1,
														"numoutlets" : 0,
														"patching_rect" : [ 228.0, 418.0, 35.0, 22.0 ],
														"id" : "obj-5"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "in 1",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 50.0, 14.0, 28.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-1"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "in 2",
														"numinlets" : 0,
														"numoutlets" : 1,
														"patching_rect" : [ 230.5, 14.0, 28.0, 22.0 ],
														"outlettype" : [ "" ],
														"id" : "obj-2"
													}

												}
, 												{
													"box" : 													{
														"maxclass" : "newobj",
														"text" : "out 1",
														"numinlets" : 1,
														"numoutlets" : 0,
														"patching_rect" : [ 50.0, 418.0, 35.0, 22.0 ],
														"id" : "obj-4"
													}

												}
 ],
											"lines" : [ 												{
													"patchline" : 													{
														"source" : [ "obj-31", 0 ],
														"destination" : [ "obj-35", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-9", 0 ],
														"destination" : [ "obj-8", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-1", 0 ],
														"destination" : [ "obj-6", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-2", 0 ],
														"destination" : [ "obj-7", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-8", 0 ],
														"destination" : [ "obj-7", 1 ],
														"midpoints" : [ 287.5, 78.0, 251.0, 78.0 ],
														"order" : 0
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-8", 0 ],
														"destination" : [ "obj-6", 1 ],
														"midpoints" : [ 287.5, 78.0, 70.0, 78.0 ],
														"order" : 1
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-11", 0 ],
														"destination" : [ "obj-12", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-6", 0 ],
														"destination" : [ "obj-11", 0 ],
														"midpoints" : [ 59.5, 136.0, 140.5, 136.0 ],
														"order" : 0
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-7", 0 ],
														"destination" : [ "obj-11", 0 ],
														"midpoints" : [ 240.5, 136.0, 140.5, 136.0 ],
														"order" : 1
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-12", 0 ],
														"destination" : [ "obj-13", 0 ],
														"order" : 1
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-6", 0 ],
														"destination" : [ "obj-15", 0 ],
														"order" : 1
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-15", 0 ],
														"destination" : [ "obj-4", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-7", 0 ],
														"destination" : [ "obj-16", 0 ],
														"order" : 0
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-16", 0 ],
														"destination" : [ "obj-5", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-13", 0 ],
														"destination" : [ "obj-17", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-12", 0 ],
														"destination" : [ "obj-20", 0 ],
														"midpoints" : [ 140.5, 209.0, 239.5, 209.0, 239.5, 145.0, 338.5, 145.0 ],
														"order" : 0
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-14", 0 ],
														"destination" : [ "obj-20", 1 ],
														"midpoints" : [ 507.5, 95.5, 349.0, 95.5 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-19", 0 ],
														"destination" : [ "obj-21", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-20", 0 ],
														"destination" : [ "obj-22", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-22", 0 ],
														"destination" : [ "obj-23", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-23", 0 ],
														"destination" : [ "obj-24", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-21", 0 ],
														"destination" : [ "obj-25", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-25", 0 ],
														"destination" : [ "obj-22", 1 ],
														"midpoints" : [ 446.5, 199.5, 349.0, 199.5 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-27", 0 ],
														"destination" : [ "obj-16", 1 ],
														"midpoints" : [ 140.5, 380.5, 248.0, 380.5 ],
														"order" : 0
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-24", 0 ],
														"destination" : [ "obj-29", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-29", 0 ],
														"destination" : [ "obj-17", 1 ],
														"midpoints" : [ 338.5, 335.0, 284.75, 335.0, 284.75, 244.0, 151.0, 244.0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-17", 0 ],
														"destination" : [ "obj-18", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-18", 0 ],
														"destination" : [ "obj-27", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-27", 0 ],
														"destination" : [ "obj-15", 1 ],
														"midpoints" : [ 140.5, 377.5, 70.0, 377.5 ],
														"order" : 1
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-32", 0 ],
														"destination" : [ "obj-33", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-33", 0 ],
														"destination" : [ "obj-34", 1 ],
														"order" : 1
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-33", 0 ],
														"destination" : [ "obj-35", 1 ],
														"order" : 0
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-30", 0 ],
														"destination" : [ "obj-34", 0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-34", 0 ],
														"destination" : [ "obj-27", 1 ],
														"midpoints" : [ 417.5, 409.0, 272.25, 409.0, 272.25, 316.0, 151.0, 316.0 ]
													}

												}
, 												{
													"patchline" : 													{
														"source" : [ "obj-35", 0 ],
														"destination" : [ "obj-27", 2 ],
														"midpoints" : [ 628.5, 409.0, 273.0, 409.0, 273.0, 316.0, 161.5, 316.0 ]
													}

												}
 ]
										}

									}
,
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "signal", "signal" ],
									"patching_rect" : [ 119.0, 104.0, 135.0, 23.0 ],
									"rnbo_classname" : "gen~",
									"rnbo_extra_attributes" : 									{
										"exposeparams" : 0
									}
,
									"rnbo_serial" : 1,
									"rnbo_uniqueid" : "Compressor",
									"rnboinfo" : 									{
										"needsInstanceInfo" : 1,
										"argnames" : 										{
											"in1" : 											{
												"attrOrProp" : 1,
												"digest" : "in1",
												"isalias" : 0,
												"aliases" : [  ],
												"attachable" : 0,
												"isparam" : 0,
												"deprecated" : 0,
												"inlet" : 1,
												"type" : "number"
											}
,
											"reset" : 											{
												"attrOrProp" : 1,
												"digest" : "Reset all param and history objects to initial values",
												"isalias" : 0,
												"aliases" : [  ],
												"attachable" : 1,
												"isparam" : 0,
												"deprecated" : 0,
												"type" : "bang"
											}
,
											"expr" : 											{
												"attrOrProp" : 2,
												"digest" : "a gen expression",
												"isalias" : 0,
												"aliases" : [  ],
												"settable" : 1,
												"attachable" : 0,
												"isparam" : 0,
												"deprecated" : 0,
												"type" : "symbol",
												"doNotShowInMaxInspector" : 1
											}
,
											"file" : 											{
												"attrOrProp" : 2,
												"digest" : "gendsp file to load",
												"isalias" : 0,
												"aliases" : [  ],
												"settable" : 1,
												"attachable" : 0,
												"isparam" : 0,
												"deprecated" : 0,
												"type" : "symbol",
												"doNotShowInMaxInspector" : 1
											}
,
											"title" : 											{
												"attrOrProp" : 2,
												"digest" : "a title",
												"defaultarg" : 1,
												"isalias" : 0,
												"aliases" : [ "t" ],
												"settable" : 1,
												"attachable" : 0,
												"isparam" : 0,
												"deprecated" : 0,
												"type" : "symbol",
												"doNotShowInMaxInspector" : 1
											}
,
											"t" : 											{
												"attrOrProp" : 2,
												"digest" : "a title",
												"defaultarg" : 1,
												"isalias" : 1,
												"aliasOf" : "title",
												"aliases" : [  ],
												"settable" : 1,
												"attachable" : 0,
												"isparam" : 0,
												"deprecated" : 0,
												"type" : "symbol"
											}
,
											"exposeparams" : 											{
												"attrOrProp" : 2,
												"digest" : "Expose gen params as RNBO params.",
												"isalias" : 0,
												"aliases" : [  ],
												"settable" : 1,
												"attachable" : 0,
												"isparam" : 0,
												"deprecated" : 0,
												"type" : "bool",
												"defaultValue" : "false"
											}

										}
,
										"inputs" : [ 											{
												"name" : "in1",
												"type" : "auto",
												"digest" : "in1",
												"hot" : 1,
												"docked" : 0
											}
, 											{
												"name" : "in2",
												"type" : "auto"
											}
 ],
										"outputs" : [ 											{
												"name" : "out1",
												"type" : "signal"
											}
, 											{
												"name" : "out2",
												"type" : "signal"
											}
 ],
										"helpname" : "gen~",
										"aliasOf" : "gen~",
										"classname" : "gen~",
										"operator" : 0,
										"versionId" : 1405647718,
										"changesPatcherIO" : 0
									}
,
									"text" : "gen~ @title Compressor",
									"varname" : "Compressor"
								}

							}
 ],
						"lines" : [  ],
						"default_bgcolor" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
						"color" : [ 0.929412, 0.929412, 0.352941, 1.0 ],
						"elementcolor" : [ 0.357540726661682, 0.515565991401672, 0.861786782741547, 1.0 ],
						"accentcolor" : [ 0.343034118413925, 0.506230533123016, 0.86220508813858, 1.0 ],
						"stripecolor" : [ 0.258338063955307, 0.352425158023834, 0.511919498443604, 1.0 ],
						"bgfillcolor_type" : "color",
						"bgfillcolor_color" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
						"bgfillcolor_color1" : [ 0.031372549019608, 0.125490196078431, 0.211764705882353, 1.0 ],
						"bgfillcolor_color2" : [ 0.263682, 0.004541, 0.038797, 1.0 ],
						"bgfillcolor_angle" : 270.0,
						"bgfillcolor_proportion" : 0.39,
						"bgfillcolor_autogradient" : 0.0
					}
,
					"patching_rect" : [ 155.0, 87.0, 40.0, 22.0 ],
					"rnboattrcache" : 					{

					}
,
					"rnboversion" : "1.2.6",
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_invisible" : 1,
							"parameter_longname" : "rnbo~",
							"parameter_modmode" : 0,
							"parameter_shortname" : "rnbo~",
							"parameter_type" : 3
						}

					}
,
					"saved_object_attributes" : 					{
						"optimization" : "O1",
						"parameter_enable" : 1,
						"uuid" : "84416817-fce2-11ee-a817-367dda5e532f"
					}
,
					"snapshot" : 					{
						"filetype" : "C74Snapshot",
						"version" : 2,
						"minorversion" : 0,
						"name" : "snapshotlist",
						"origin" : "rnbo~",
						"type" : "list",
						"subtype" : "Undefined",
						"embed" : 1,
						"snapshot" : 						{
							"__presetid" : "84416817-fce2-11ee-a817-367dda5e532f"
						}
,
						"snapshotlist" : 						{
							"current_snapshot" : 0,
							"entries" : [ 								{
									"filetype" : "C74Snapshot",
									"version" : 2,
									"minorversion" : 0,
									"name" : "untitled",
									"origin" : "84416817-fce2-11ee-a817-367dda5e532f",
									"type" : "rnbo",
									"subtype" : "",
									"embed" : 0,
									"snapshot" : 									{
										"__presetid" : "84416817-fce2-11ee-a817-367dda5e532f"
									}
,
									"fileref" : 									{
										"name" : "untitled",
										"filename" : "untitled_20240417.maxsnap",
										"filepath" : "~/Documents/Max 8/Snapshots",
										"filepos" : -1,
										"snapshotfileid" : "2ab31bfdc76643857694e210b1905087"
									}

								}
 ]
						}

					}
,
					"text" : "rnbo~",
					"varname" : "rnbo~"
				}

			}
 ],
		"lines" : [  ],
		"parameters" : 		{
			"obj-1" : [ "rnbo~", "rnbo~", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "untitled_20240417.maxsnap",
				"bootpath" : "~/Documents/Max 8/Snapshots",
				"patcherrelativepath" : "../../../../Documents/Max 8/Snapshots",
				"type" : "mx@s",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
