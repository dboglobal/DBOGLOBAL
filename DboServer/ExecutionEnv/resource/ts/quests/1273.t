CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1273;
	title = 127302;

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
				conv = 127309;
				ctype = 1;
				idx = 4372101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 127308;
				gtype = 1;
				area = 127301;
				goal = 127304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 127305;
				stype = 1;
				taid = 1;
				title = 127302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 127307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1288;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4372101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 127308;
			gtype = 1;
			oklnk = 2;
			area = 127301;
			goal = 127304;
			sort = 127305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 127302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 127314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 127301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7611203;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

