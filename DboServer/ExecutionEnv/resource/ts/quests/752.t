CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 752;
	title = 75202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 5;
			prelnk = "2;";

			CDboTSActItem
			{
				iidx0 = 99031;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 3;
			prelnk = "7;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 5774200;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
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
			nextlnk = 7;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 75209;
				ctype = 1;
				idx = 3173107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 75208;
				area = 75201;
				goal = 75204;
				m0fz = "-465.000000";
				m0widx = 1;
				scitem = -1;
				sort = 75205;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "5272.000000";
				m0ttip = 75215;
				rwd = 100;
				taid = 1;
				title = 75202;
				gtype = 3;
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
			desc = 75214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 75201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751104;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 75208;
			gtype = 3;
			oklnk = 2;
			area = 75201;
			goal = 75204;
			sort = 75205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 75202;
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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 75207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 25;
			}
			CDboTSCheckClrQst
			{
				and = "751;";
				flink = 1;
				flinknextqid = "753;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4754203;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActItem
			{
				iidx0 = 99031;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99031;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;2;";
			type = 0;
		}
	}
}

