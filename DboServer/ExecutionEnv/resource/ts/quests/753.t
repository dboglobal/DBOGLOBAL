CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 753;
	title = 75302;

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
			cid = 4;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 5931100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 75314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 75301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751104;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 75309;
				ctype = 1;
				idx = 4751104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 75308;
				area = 75301;
				goal = 75304;
				m0fz = "-468.130005";
				m0widx = 1;
				scitem = -1;
				sort = 75305;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "5277.310059";
				m0ttip = 75316;
				rwd = 100;
				taid = 1;
				title = 75302;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 75308;
			gtype = 2;
			oklnk = 2;
			area = 75301;
			goal = 75304;
			sort = 75305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 75302;
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
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "4;";
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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 75307;
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
			CDboTSClickNPC
			{
				npcidx = "4751104;";
			}
			CDboTSCheckClrQst
			{
				and = "752;";
				flink = 1;
				flinknextqid = "754;";
				not = 0;
			}
		}
	}
}

