CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 210;
	title = 21002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 21009;
				ctype = 1;
				idx = 5041148;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 21008;
				gtype = 1;
				area = 21001;
				goal = 21004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 21005;
				stype = 64;
				taid = 1;
				title = 21002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 21007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "5041148;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "211;";
				not = 0;
				or = "209;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 21008;
			gtype = 1;
			oklnk = 2;
			area = 21001;
			goal = 21004;
			sort = 21005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 21002;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			desc = 21014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 21001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8411103;";
			}
		}
	}
}

