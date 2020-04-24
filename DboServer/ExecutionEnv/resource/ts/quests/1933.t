CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1933;
	title = 193302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 193309;
				ctype = 1;
				idx = 1655301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 193308;
				m1fx = "-173.869995";
				m1fy = "0.000000";
				sort = 193305;
				m0fy = "3.550000";
				m0pat = 2;
				m1fz = "397.809998";
				grade = 132203;
				m0fx = "13.930000";
				m0ttip = 193315;
				m1pat = 2;
				m1widx = 4;
				rwd = 100;
				taid = 1;
				title = 193302;
				gtype = 2;
				area = 193301;
				goal = 193304;
				m0fz = "177.679993";
				m0widx = 4;
				m1ttip = 193315;
				scitem = -1;
				stype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1225;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 193308;
			gtype = 2;
			oklnk = 2;
			area = 193301;
			goal = 193304;
			sort = 193305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 193302;
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
			desc = 193314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 193301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "1655301;";
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 193307;
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
				npcidx = "1655301;";
			}
			CDboTSCheckClrQst
			{
				and = "1087;";
				flink = 0;
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1225;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 193307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

