CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1775;
	title = 177502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 205;
			elnk = 255;
			nextlnk = 254;
			prelnk = "204;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 7;
				osh_sh = 1;
				widx = 13;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "201;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 7;
				osh_sh = 255;
				widx = 13;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 6;
				osh_sh = 255;
				widx = 13;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 201;
			prelnk = "9;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 6;
				osh_sh = 255;
				widx = 13;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 202;
			prelnk = "10;";

			CDboTSActObjState
			{
				mstate = 3;
				oidx = 6;
				osh_sh = 0;
				widx = 13;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "205;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 204;
			prelnk = "202;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 60000;
			}
		}
		CDboTSContGAct
		{
			cid = 204;
			elnk = 255;
			nextlnk = 205;
			prelnk = "203;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 6;
				osh_sh = 1;
				widx = 13;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActCustomEvt
			{
				ceid = 177501;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 5;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 177507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckCustomEvent
			{
				qid = 1775;
			}
			CDboTSCheckAttachObj
			{
				objidx = "6;";
				widx = 13;
			}
		}
	}
}

