CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1092;
	title = 109202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
			CDboTSActItem
			{
				iidx0 = 99033;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 109214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 109201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1655201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 109208;
			gtype = 2;
			oklnk = 2;
			area = 109201;
			goal = 109204;
			sort = 109205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 109202;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 109207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "1655201;";
			}
			CDboTSCheckClrQst
			{
				and = "1082;";
				flink = 0;
				not = 0;
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
				conv = 109209;
				ctype = 1;
				idx = 1655201;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99033;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 109208;
				m1fx = "-56.000000";
				m2fz = "-199.000000";
				m1fy = "0.000000";
				sort = 109205;
				m2widx = 4;
				grade = 132203;
				m1pat = 0;
				m1widx = 4;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 109201;
				goal = 109204;
				m0fz = "-183.000000";
				m0widx = 4;
				m1ttip = 109215;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "-189.000000";
				m2fx = "-11.000000";
				m2ttip = 109215;
				m0fx = "-91.000000";
				m0ttip = 109215;
				title = 109202;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 191;
				cnt0 = 3;
				ectype = -1;
				etype = 0;
				qtidx = 109230;
				taid = 1;
				esctype = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 109207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99033;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

