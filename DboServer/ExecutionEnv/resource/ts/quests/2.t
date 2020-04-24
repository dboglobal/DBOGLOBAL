CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 2;
	title = 202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 103;
			elnk = 255;
			nextlnk = 254;
			prelnk = "102;";

			CDboTSActQItem
			{
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 102;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 103;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 0;
				iidx = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "102;103;";
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
			stdiag = 207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "3221202;";
			}
			CDboTSCheckPCCls
			{
				clsflg = 2;
			}
			CDboTSCheckClrQst
			{
				and = "1;";
				flink = 1;
				flinknextqid = "3;";
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
				conv = 209;
				ctype = 1;
				idx = 3221202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 208;
				m1fx = "4871.439941";
				m1fy = "0.000000";
				sort = 205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "4063.719971";
				grade = 132203;
				m0fx = "4602.169922";
				m0ttip = 215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 202;
				gtype = 2;
				area = 201;
				goal = 204;
				m0fz = "4336.029785";
				m0widx = 1;
				m1ttip = 215;
				scitem = -1;
				stype = 1;
			}
			CDboTSActOpenWindow
			{
				show = 1;
				taid = 4;
				tblidx = 20002;
				wtype = 5;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 4911100;
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
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3221202;";
			}
			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 0;
				iidx = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 208;
			gtype = 2;
			oklnk = 2;
			area = 201;
			goal = 204;
			sort = 205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 202;
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
	}
}

