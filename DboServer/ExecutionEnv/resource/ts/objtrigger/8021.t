CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 8021;
	title = 802102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 31;
			prelnk = "21;";
			nolnk = 41;
			rm = 0;
			yeslnk = 32;

			CDboTSCheckObjState
			{
				mstate = 2;
				oidx = 7;
				osh = 255;
				widx = 310000;
			}
		}
		CDboTSContGAct
		{
			cid = 35;
			elnk = 255;
			nextlnk = 254;
			prelnk = "34;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 3;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 7;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 34;
			elnk = 255;
			nextlnk = 35;
			prelnk = "33;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 19;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 2;
				oidx = 20;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 54;
			elnk = 255;
			nextlnk = 254;
			prelnk = "53;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 7;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 32;
			elnk = 255;
			nextlnk = 33;
			prelnk = "31;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 19;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 20;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 33;
			elnk = 255;
			nextlnk = 34;
			prelnk = "32;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 44;
			elnk = 255;
			nextlnk = 254;
			prelnk = "43;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 3;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 52;
			elnk = 255;
			nextlnk = 53;
			prelnk = "51;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 53;
			elnk = 255;
			nextlnk = 54;
			prelnk = "52;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 20;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 52;
			prelnk = "22;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 20;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 23;
			elnk = 255;
			nextlnk = 21;
			prelnk = "22;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGCond
		{
			cid = 22;
			prelnk = "21;";
			nolnk = 23;
			rm = 0;
			yeslnk = 51;

			CDboTSCheckObjState
			{
				mstate = 2;
				oidx = 7;
				osh = 255;
				widx = 310000;
			}
		}
		CDboTSContGAct
		{
			cid = 43;
			elnk = 255;
			nextlnk = 44;
			prelnk = "42;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 19;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 42;
			elnk = 255;
			nextlnk = 43;
			prelnk = "41;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 41;
			elnk = 255;
			nextlnk = 42;
			prelnk = "31;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 19;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 21;
			prelnk = "11;23;";
			nolnk = 22;
			rm = 0;
			yeslnk = 31;

			CDboTSCheckObjState
			{
				mstate = 2;
				oidx = 3;
				osh = 255;
				widx = 310000;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 21;
			prelnk = "10;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 11;
			prelnk = "0;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "35;54;44;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 802107;
			nolnk = 255;
			rm = 0;
			yeslnk = 10;

			CDboTSCheckAttachObj
			{
				objidx = "3;7;";
				widx = 310000;
			}
		}
	}
}

