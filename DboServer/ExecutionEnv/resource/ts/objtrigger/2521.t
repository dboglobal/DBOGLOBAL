CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2521;
	title = 252102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 11;
			prelnk = "10;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
			CDboTSActObjState
			{
				mstate = 2;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 255;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "11;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 30000;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "15;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 252118;
			}
			CDboTSActCustomEvt
			{
				ceid = 252101;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 15;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 254;
			prelnk = "12;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;13;";
			type = 1;
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
			nextlnk = 10;
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
			stdiag = 252107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckCustomEvent
			{
				qid = 2521;
			}
			CDboTSCheckAttachObj
			{
				objidx = "170;171;172;173;174;175;176;177;178;179;";
				widx = 14;
			}
		}
	}
}

