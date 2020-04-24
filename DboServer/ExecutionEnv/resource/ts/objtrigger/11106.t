CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11106;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 8;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckHasCoupon
			{
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 253;
			prelnk = "9;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 52;
				osh_sh = 255;
				widx = 191000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "1;";

			CDboTSActObjConv
			{
				conv = 1100928;
				idx = 52;
				widx = 191000;
				ctype = 1;
				taid = 2;
			}
			CDboTSActObjState
			{
				mstate = 2;
				oidx = 52;
				osh_sh = 255;
				widx = 191000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "52;";
				widx = 191000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActRmvCoupon
			{
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "10;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActDrop
			{
				ptype = 1;
				taid = 1;
				tblidx = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActObjConv
			{
				conv = 1100929;
				idx = 52;
				widx = 191000;
				ctype = 1;
				taid = 2;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 52;
				osh_sh = 255;
				widx = 191000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 52;
				osh_sh = 255;
				widx = 191000;
				osh_uc = 255;
				taid = 1;
			}
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
				time = 3000;
			}
		}
	}
}

