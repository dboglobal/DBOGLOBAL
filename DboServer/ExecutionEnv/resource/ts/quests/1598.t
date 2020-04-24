CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 6;
	rq = 0;
	tid = 1598;
	title = 159802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 101;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 1598;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 159807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "6313106;";
			}
			CDboTSCheckClrQst
			{
				and = "1563;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 159808;
			gtype = 2;
			oklnk = 2;
			area = 159801;
			goal = 159804;
			sort = 159805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 159802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 159814;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 159801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6313106;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "4;2;";
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 1598;
				start = 1;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 159809;
				ctype = 1;
				idx = 6313106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 159808;
				gtype = 2;
				area = 159801;
				goal = 159804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 159805;
				stype = 2;
				taid = 1;
				title = 159802;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 806;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 159820;
				taid = 3;
				widx0 = 1;
				dt0 = 159821;
				esctype = 0;
				otype0 = 7;
			}
		}
	}
}

