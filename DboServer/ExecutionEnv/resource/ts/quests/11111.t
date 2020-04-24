CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11111;
	title = 1100849;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100856;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 5061401;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 252;
			nextlnk = 3;
			prelnk = "240;";

			CDboTSActNPCConv
			{
				conv = 1100852;
				ctype = 1;
				idx = 5061401;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				eitype = 6;
				tidx0 = 2951201;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 1100853;
				taid = 3;
				widx0 = 191000;
				dt0 = 1100854;
				esctype = 0;
				otype0 = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100851;
				gtype = 1;
				area = 1100807;
				goal = 1100850;
				grade = 132203;
				rwd = 100;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100849;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			didx0 = -1;
			dval0 = 500;
			desc = 1100855;
			nextlnk = 101;
			rwdtbl = 0;
			rwdtype = 0;
			dtype0 = 4;
			ltime = -1;
			prelnk = "3;";
			usetbl = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 240;
			prelnk = "4;";
			nolnk = 5;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 191000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 3;
			prelnk = "240;";

			CDboTSActNPCConv
			{
				conv = 1100852;
				ctype = 1;
				idx = 5061401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 1100851;
				gtype = 1;
				area = 1100807;
				goal = 1100850;
				grade = 132203;
				rwd = 100;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100849;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				eitype = 6;
				tidx0 = 2951201;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 1100853;
				taid = 3;
				widx0 = 60000;
				dt0 = 1100854;
				esctype = 0;
				otype0 = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "241;5;2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "4;";

			CDboTSActNPCConv
			{
				conv = 1100852;
				ctype = 1;
				idx = 5061401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 1100851;
				gtype = 1;
				area = 1100807;
				goal = 1100850;
				grade = 132203;
				rwd = 100;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100849;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				eitype = 6;
				tidx0 = 2951201;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 1100853;
				taid = 3;
				widx0 = 11000;
				dt0 = 1100854;
				esctype = 0;
				otype0 = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1100851;
			gtype = 1;
			oklnk = 4;
			area = 1100807;
			goal = 1100850;
			sort = 1100024;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 1100849;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "1;";
			nolnk = 240;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckInWorld
			{
				widx = 11000;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "241;5;2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
}

