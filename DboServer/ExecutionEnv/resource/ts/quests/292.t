CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 1;
	rq = 0;
	tid = 292;
	title = 29202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 292;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 292;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;3;2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 29209;
				ctype = 1;
				idx = 4751201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 29208;
				area = 29201;
				goal = 29204;
				m0fz = "-2386.510010";
				m0widx = 1;
				scitem = -1;
				sort = 29205;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4801.569824";
				m0ttip = 29215;
				rwd = 100;
				taid = 1;
				title = 29202;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 29208;
			gtype = 2;
			oklnk = 2;
			area = 29201;
			goal = 29204;
			sort = 29205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 29202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 29214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 29201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6312101;";
			}
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
			stdiag = 29207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 3;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 14;
			}
			CDboTSClickNPC
			{
				npcidx = "4751201;";
			}
			CDboTSCheckClrQst
			{
				and = "281;";
				flink = 1;
				flinknextqid = "300;";
				not = 0;
			}
		}
	}
}

