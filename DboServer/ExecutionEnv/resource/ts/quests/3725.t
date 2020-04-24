CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3725;
	title = 372502;

	CNtlTSGroup
	{
		gid = 0;

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
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372509;
				ctype = 1;
				idx = 7076104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 40;
				eitype = 0;
				idx0 = 14521104;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 18481103;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 372508;
				m3widx = 5;
				m1fy = "-58.599998";
				sort = 372505;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-3501.520020";
				rwd = 100;
				taid = 1;
				gtype = 1;
				m3fy = "-63.709999";
				area = 372501;
				goal = 372504;
				m0fz = "-4839.129883";
				m0widx = 15;
				m1ttip = 372515;
				m2fy = "-56.849998";
				m2pat = 2;
				m3fz = "-4809.020020";
				m3ttip = 372516;
				scitem = -1;
				stype = 2;
				m0fy = "-66.419998";
				m0pat = 2;
				m1fz = "-4873.229980";
				m2fx = "-3339.979980";
				m2ttip = 372516;
				m0fx = "-3488.080078";
				m0ttip = 372515;
				m3pat = 2;
				title = 372502;
				m1fx = "-3340.070068";
				m2fz = "-4884.899902";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372507;
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
				npcidx = "7076104;";
			}
			CDboTSCheckClrQst
			{
				and = "3724;";
				flink = 0;
				flinknextqid = "3726;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372514;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 372501;
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
			cont = 372508;
			gtype = 1;
			oklnk = 2;
			area = 372501;
			goal = 372504;
			sort = 372505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372502;
		}
	}
}

