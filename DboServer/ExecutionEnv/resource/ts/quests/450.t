CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 450;
	title = 45002;

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
				conv = 45009;
				ctype = 1;
				idx = 4511304;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 45008;
				gtype = 1;
				area = 45001;
				goal = 45004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 45005;
				stype = 2;
				taid = 1;
				title = 45002;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 45014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 45001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 45007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 9;
			}
			CDboTSClickNPC
			{
				npcidx = "4511304;";
			}
			CDboTSCheckClrQst
			{
				and = "448;";
				flink = 1;
				flinknextqid = "454;";
				not = 0;
			}
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
			cont = 45008;
			gtype = 1;
			oklnk = 2;
			area = 45001;
			goal = 45004;
			sort = 45005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 45002;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

