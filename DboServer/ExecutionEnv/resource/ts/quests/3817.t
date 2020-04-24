CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 24;
	rq = 0;
	tid = 3817;
	title = 381702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 381707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "1161101;";
			}
			CDboTSCheckClrQst
			{
				and = "3745;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 381709;
				ctype = 1;
				idx = 1161101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 13811101;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 381708;
				area = 381701;
				goal = 381704;
				m0fz = "-4718.979980";
				m0widx = 15;
				scitem = -1;
				sort = 381705;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "-4988.649902";
				m0ttip = 381715;
				rwd = 100;
				taid = 1;
				title = 381702;
				gtype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 381708;
			gtype = 2;
			oklnk = 2;
			area = 381701;
			goal = 381704;
			sort = 381705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 381702;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 381714;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 381701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1161101;";
			}
		}
	}
}

