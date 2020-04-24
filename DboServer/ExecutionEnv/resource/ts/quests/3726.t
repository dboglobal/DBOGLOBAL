CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3726;
	title = 372602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372609;
				ctype = 1;
				idx = 4952202;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 14094200;
				cnt0 = 70;
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
				cont = 372608;
				m1fx = "-3431.719971";
				m2fz = "-5193.430176";
				m1fy = "-110.959999";
				sort = 372605;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 372601;
				goal = 372604;
				m0fz = "-5093.319824";
				m0widx = 15;
				m1ttip = 372615;
				m2fy = "-90.629997";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "-74.550003";
				m0pat = 2;
				m1fz = "-5358.899902";
				m2fx = "-3423.949951";
				m2ttip = 372615;
				m0fx = "-3351.879883";
				m0ttip = 372615;
				title = 372602;
			}
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372607;
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
				npcidx = "4952202;";
			}
			CDboTSCheckClrQst
			{
				and = "3725;";
				flink = 0;
				flinknextqid = "3727;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372614;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 372601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4952202;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372608;
			gtype = 2;
			oklnk = 2;
			area = 372601;
			goal = 372604;
			sort = 372605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372602;
		}
	}
}

